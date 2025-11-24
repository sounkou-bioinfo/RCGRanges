#!/usr/bin/env Rscript
# Fetch CGRanges from Heng Li's repository
# and put it in inst/cgranges
hash <- "b3d5e2c5b9a0a379f54592ab85f6cff5d58c387e"
files <- c(
  "cgranges.h",
  "cgranges.c",
  "khash.h",
  "README.md"
)
base_url <- "https://raw.githubusercontent.com/lh3/cgranges/"
dest_dir <- "../src"
getScriptPath <- function() {
  cmd.args <- commandArgs()
  m <- regexpr("(?<=^--file=).+", cmd.args, perl = TRUE)
  script.dir <- dirname(regmatches(cmd.args, m))
  if (length(script.dir) == 0) {
    stop("can't determine script dir: please call the script with Rscript")
  }
  if (length(script.dir) > 1) {
    stop("can't determine script dir: more than one '--file' argument detected")
  }
  return(script.dir)
}
# get this script's directory
script_dir <- getScriptPath()
dest_dir <- file.path(script_dir, dest_dir) |> normalizePath()
message("Downloading CGRanges files to ", dest_dir)
if (!dir.exists(dest_dir)) {
  dir.create(dest_dir, recursive = TRUE)
}
for (file in files) {
  url <- paste0(base_url, hash, "/", file)
  dest_file <- file.path(dest_dir, file)
  download.file(url, dest_file, quiet = TRUE)
  message("Downloaded ", file, " to ", dest_file)
}
