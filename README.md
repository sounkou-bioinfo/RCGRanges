
# RCGRanges

A minimal R interface to [cgranges](https://github.com/lh3/cgranges) by
Heng Li, for fast interval and range queries in genomics.

## Installation

``` r
# Install
remotes::install_github("sounkou-bionfo/RCGRanges")
```

## Usage

``` r

library(RCGRanges)

# Create a CGRanges object
gr <- CGRanges()

# Add intervals
grnms <- c("chr1", "chr2")
starts <- c(100L, 200L)
ends <- c(150L, 250L)
labels <- c(1L, 2L)
gr
#> <RCGRanges::CGRanges>
#>  @ .ptr    :<externalptr> 
#>  @ indexed : logi FALSE
#>  @ contigs :List of 2
#>  .. $ : chr(0) 
#>  .. $ : int(0) 
#>  @ ranges  :List of 5
#>  .. $ : chr(0) 
#>  .. $ : int(0) 
#>  .. $ : int(0) 
#>  .. $ : int(0) 
#>  .. $ : NULL
#>  @ nranges : num 0
#>  @ ncontigs: int 0
gr@.ptr
#> <pointer: 0x628482215900>

addIntervals(gr, contig_names = grnms, starts = starts, ends = ends, labels = labels)
#> NULL

# Index intervals
gr <- index(gr)
gr
#> <RCGRanges::CGRanges>
#>  @ .ptr    :<externalptr> 
#>  @ indexed : logi TRUE
#>  @ contigs :List of 2
#>  .. $ : chr [1:2] "chr1" "chr2"
#>  .. $ : int [1:2] 150 250
#>  @ ranges  :List of 5
#>  .. $ : chr [1:2] "chr1" "chr2"
#>  .. $ : int [1:2] 100 200
#>  .. $ : int [1:2] 150 250
#>  .. $ : int [1:2] 1 2
#>  .. $ : NULL
#>  @ nranges : num 2
#>  @ ncontigs: int 2
gr@.ptr
#> <pointer: 0x628482215900>

# Create second CGRanges object
gr2 <- CGRanges()
addIntervals(gr2, contig_names = c("chr1", "chr2"), starts = c(120L, 210L), ends = c(160L, 260L), labels = c(3L, 4L))
#> NULL
gr2 <- index(gr2)


# Find overlaps between gr and gr2 (indices are 1-based in R)
overlaps <- overlap(gr, gr2)
overlaps_1based <- lapply(overlaps, function(x) lapply(x, function(idx) idx + 1))
print(overlaps_1based)
#> [[1]]
#> [[1]][[1]]
#> [1] 2
#> 
#> 
#> [[2]]
#> [[2]][[1]]
#> [1] 3

# Example: non-overlapping intervals
gr3 <- CGRanges()
addIntervals(gr3, contig_names = c("chr1", "chr2"), starts = c(300L, 400L), ends = c(350L, 450L), labels = c(5L, 6L))
#> NULL
gr3 <- index(gr3)
overlaps_none <- overlap(gr, gr3)
overlaps_none_1based <- lapply(overlaps_none, function(x) lapply(x, function(idx) idx + 1))
print(overlaps_none_1based)
#> [[1]]
#> [[1]][[1]]
#> numeric(0)
#> 
#> 
#> [[2]]
#> [[2]][[1]]
#> numeric(0)
```

## Reference

- [cgranges by Heng Li](https://github.com/lh3/cgranges)
