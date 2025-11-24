#' @useDynLib RCGRanges, .registration = TRUE
NULL

# enable usage of <S7_object>@name in package code
#' @rawNamespace if (getRversion() < "4.3.0") importFrom("S7", "@")
NULL

.onLoad <- function(...) {
    S7::methods_register()
}

#' CGRanges S7 Class
#' An S7 class representing a CGRanges object.
#' @export
CGRanges <- S7::new_class(
    "CGRanges",
    properties = list(
        .ptr = S7::new_property(
            default = quote(.Call(RC_cr_init)),
            setter = function(self, value) {
                if (!is.null(self@.ptr)) {
                    # warn, not error, until https://github.com/RConsortium/S7/issues/520 is fixed
                    warning(
                        "changing .ptr value not permitted. This will error in the future."
                    )
                }
                self@.ptr <- value
                self
            },
            validator = function(value) {
                if (typeof(value) != "externalptr") {
                    "must be an externalptr"
                }
            }
        ),
        indexed = S7::new_property(
            default = FALSE,
            setter = function(self, value) {
                self@indexed <- value
                self
            }
        ),
        contigs = S7::new_property(
            default = list(),
            getter = function(self) {
                .Call(RC_get_cr_contigs, self@.ptr)
            }
        ),
        ranges = S7::new_property(
            default = list(),
            getter = function(self) {
                .Call(RC_get_cr_intervals, self@.ptr, self@indexed)
            }
        ),
        nranges = S7::new_property(
            default = 0L,
            getter = function(self) {
                .Call(RC_get_cr_n_intervals, self@.ptr)
            }
        ),
        ncontigs = S7::new_property(
            default = 0L,
            getter = function(self) {
                .Call(RC_get_cr_n_contigs, self@.ptr)
            }
        )
    )
)

#' @export
addIntervals <- S7::new_generic(
    "addIntervals",
    "x",
    function(x, contig_names, starts, ends, labels, ...) {
        S7::S7_dispatch()
    }
)


#' add intervals to CGRanges object method
#' @name addIntervals
#' @export
S7::method(addIntervals, CGRanges) <- function(
    x,
    contig_names,
    starts,
    ends,
    labels) {
    .Call(
        RC_cr_add,
        x@.ptr,
        contig_names,
        starts,
        ends,
        labels
    )
    invisible(x)
}

#' Index CGRanges object
#' @name index
#' @export
index <- S7::new_generic("index", "x", function(x, ...) {
    S7::S7_dispatch()
})

#' Index method for CGRanges object
#' @name index
#' @export
S7::method(index, CGRanges) <- function(x) {
    .Call(RC_cr_index, x@.ptr)
    x@indexed <- TRUE
    invisible(x)
}

#' Find overlaps between two CGRanges objects
#' @name overlap
#' @export
overlap <- S7::new_generic("overlap", c("x", "y"), function(x, y, ...) {
    S7::S7_dispatch()
})

## Overlap method for two CGRanges objects
#' @name overlap
#' @export
S7::method(overlap, list(CGRanges, CGRanges)) <- function(x, y) {
    y_ranges <- y@ranges
    out_ptr <- .Call(
        RC_cr_from_overlap,
        x@.ptr,
        as.character(y_ranges[[1]]),
        as.integer(y_ranges[[2]]),
        as.integer(y_ranges[[3]])
    )
    hits <- attr(out_ptr, "hits")
    attr(out_ptr, "hits") <- NULL
    CGRangesOverlap(
        .ptr = out_ptr,
        indexed = TRUE,
        hits = hits
    )
}
#' CGRangesOverlap S7 Class
#' Inherits from CGRanges, adds 'hits' field for overlap indices
#' @export
CGRangesOverlap <- S7::new_class(
    "CGRangesOverlap",
    parent = CGRanges,
    properties = list(
        hits = S7::new_property(
            default = list(),
            validator = function(value) {
                if (!is.list(value)) {
                    "hits must be a list of integer vectors"
                }
            }
        )
    )
)
