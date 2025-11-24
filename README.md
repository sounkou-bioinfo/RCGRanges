
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
#> <pointer: 0x64c85c706c40>

addIntervals(gr, contig_names = grnms, starts = starts, ends = ends, labels = labels)

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
#> <pointer: 0x64c85c706c40>

# Create second CGRanges object
gr2 <- CGRanges()
addIntervals(gr2, contig_names = c("chr1", "chr2", "chr1"), starts = c(120L, 210L, 300L), ends = c(160L, 260L, 350L), labels = c(3L, 4L, 99L))
gr2 <- index(gr2)

# Find overlaps between gr and gr2
overlaps <- overlap(gr, gr2)
overlaps
#> <RCGRanges::CGRangesOverlap>
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
#>  @ hits    :List of 3
#>  .. $ : int 1
#>  .. $ : int(0) 
#>  .. $ : int 2
```

## Reference

- [cgranges by Heng Li](https://github.com/lh3/cgranges)
