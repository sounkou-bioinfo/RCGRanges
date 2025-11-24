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
addIntervals <- S7::new_generic("addIntervals", "x", function(x, contig_names, starts, ends, labels, ...) {
    S7::S7_dispatch()
})


#' add intervals to CGRanges object method
#' @name addIntervals
#' @export
S7::method(addIntervals, CGRanges) <- function(x, contig_names, starts, ends, labels) {
    .Call(
        RC_cr_add,
        x@.ptr,
        contig_names,
        starts,
        ends,
        labels
    )
}

#' index CGRanges object
#' @param x CGRanges object
#' @export
index <- S7::new_generic(
    "index", "x",
    function(x, ...) {
        S7::S7_dispatch()
    }
)

#' index method for CGRanges
#' @name index
#' @export
S7::method(index, CGRanges) <- function(x) {
    .Call(RC_cr_index, x@.ptr)
    x@indexed <- TRUE
    invisible(x)
}
