## Tweaks to the C++ source code are run in this R script.
## Thus running this (inside the current tools dir) after copying the
## geometry dir from s2 sources should yield a working C++ lib with R.
## IT IS IMPORTANT TO EXECUTE COMMANDS IN ORDER SO DON'T SKIP ANY!!

## Move to geometry dir:
owd <- setwd(file.path("..", "src", "geometry"))

###### GIT: Use Rcpp to handle cout, cerr and abort. #####
## Some simple tweaks using git to find matching files and sed to substitute code
system("git grep -l 'using std::cout' | xargs sed -i '/using std::cout/d'")
system("git grep -l 'std::cerr' | xargs sed -i 's/std::cerr/Rcpp::Rcerr/g'")
system("git grep -l 'cout' | xargs sed -i 's/cout/Rcpp::Rcout/g'")

## Fix abort() call and include Rcpp.h in base/logging.h
file <- file.path("base", "logging.h")
content <- readLines(file)
content <- gsub('abort()', 'Rcpp::stop("An error has occured in the C++ library!")', content, fixed = TRUE)
ii <- grep('define BASE_LOGGING_H', content)
# Add lines
content <- append(content, c('', '#include <Rcpp.h>'), ii)
writeLines(content, file)
############################################################

###### GIT: Tweak for Windows compiling. ########
## Fix Windows byte swap in base/port.h
file <- file.path("base", "port.h")
content <- readLines(file)
ii <- grep('define __BYTE_ORDER for MSVC', content)
# Overwrite next line
content[ii + 1] <- "#if defined COMPILER_MSVC || defined WIN32"
writeLines(content, file)
##################################################

### GIT: Change drem() to remainder() in geometry C++ source code ###
system("git grep -l 'drem' | grep 'cc$' | xargs sed -i 's/drem/remainder/g'")
#####################################################################

system("sed -i 's/\* children\[0\]/** children/g' s2/s2regioncoverer.h")

##### GIT: Disable warning for fabs() on clang. ######
## mathutil.cc
file <- file.path("util", "math", "mathutil.cc")
content <- readLines(file)
ii <- grep('long double const A =', content)
# Add lines
content <- c(
  content[1:(ii-1)],
  '// https://github.com/mongodb/mongo/commit/3fe2d4fb37cce8259991f7af8b58d67b357af84e',
  '// Disable error about fabs causing truncation of value because',
  '// it takes a double instead of a long double (Clang 3.5+)',
  '// See SERVER-15183',
  '#pragma clang diagnostic push',
  '#pragma clang diagnostic ignored "-Wabsolute-value"',
  '',
  content[ii:(ii+2)],
  '#pragma clang diagnostic pop',
  '',
  content[(ii+3):length(content)])
writeLines(content, file)
## mathutil.h
file <- file.path("util", "math", "mathutil.h")
content <- readLines(file)
ii <- grep('class MathUtil ', content)
# Add lines
content <- c(
  content[1:(ii-1)],
  '// https://github.com/mongodb/mongo/commit/3fe2d4fb37cce8259991f7af8b58d67b357af84e',
  '// Disable error about fabs causing truncation of value because',
  '// it takes a double instead of a long double (Clang 3.5+)',
  '// See SERVER-15183',
  '#pragma clang diagnostic push',
  '#pragma clang diagnostic ignored "-Wabsolute-value"',
  '',
  content[ii:(length(content)-1)],
  '#pragma clang diagnostic pop',
  '',
  content[length(content)])
writeLines(content, file)
######################################################


#### GIT: Fix nul character at end of string ####
file <- file.path("base", "logging.cc")
content <- readLines(file)
ii <- grep('snprintf', content)
content[ii] <- '  snprintf(buffer_, sizeof(buffer_), "%02d:%02d:%02d",'
writeLines(content, file)
#################################################

#### GIT: Fix isnan and isinf problem on Windows ####
file <- file.path("util", "math", "exactfloat", "exactfloat.cc")
content <- readLines(file)
content <- gsub("isnan", "std::isnan", content)
content <- gsub("isinf", "std::isinf", content)
writeLines(content, file)
######################################################

########### GIT: hash_map and hash_set ##############
system("git grep -l 'namespace __gnu_cxx' | xargs sed -i 's/namespace __gnu_cxx/namespace std/g'")
files <- system("git grep -l '<hash_...>'", intern = TRUE)
remove_chunk <- function(file){
  content <- readLines(file)
  ii <- grep("<hash_...>", content)
  # Lines to delete
  ii <- rep(ii, each = 5) + rep((-3):1, length(ii))
  writeLines(content[-ii], file)
}
lapply(files, remove_chunk)

files <- system("git grep -l 'using __gnu_cxx::hash_...;'", intern = TRUE)
change_chunk <- function(file, type){
  content <- readLines(file)
  ii <- grep(paste0("using __gnu_cxx::hash_", type, ";"), content)
  if(length(ii)>0){
    content <- append(content,
                      c(paste0('#include <unordered_', type, '>'), paste0("using std::unordered_", type, ";")),
                      ii)
    writeLines(content[-ii], file)
  }
}
lapply(files, change_chunk, type = "map")
lapply(files, change_chunk, type = "set")

system("git grep -l 'hash_map<' | xargs sed -i 's/hash_map</unordered_map</g'")
system("git grep -l 'hash_set<' | xargs sed -i 's/hash_set</unordered_set</g'")

## Tweak util/hash/hash.h
file <- file.path("util", "hash", "hash.h")
content <- readLines(file)
ii <- grep('__gnu_cxx::hash', content)
content <- content[-ii]
ii <- grep('ext/hash', content)
content <- content[-ii]
i1 <- grep('--------- STL hashers ---------', content)
i2 <- grep('------- Fingerprints --------', content)
i2 <- i2 - 1
writeLines(content[-(i1:i2)], file)
###################################################

setwd(owd)