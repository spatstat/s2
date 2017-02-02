as.s2poly <- function(x, ..., xylab = FALSE){
  nloop <- x$num_loops()
  loops <- list()
  sign <- rep(0, nloop)
  for(i in 1:nloop){
    loop <- x$loop(i-1)
    sign[i] <- loop$sign()
    loops[[i]] <- loop$dump()
    if(xylab) names(loops[[i]])[1:2] <- c("x", "y")
  }
  rslt <- list(loops = loops, sign = sign)
  class(rslt) <- "s2poly"
  return(rslt)
}

print.s2poly <- function(x, ...){
  cat("An s2 polygon.")
}

plotloop <- function(x, ..., col = c("green", "red"), sign = 1){
  loop <- rbind(x, x[1,])
  globe::globelines(loc = loop, ..., col = ifelse(sign>0, col[1], col[2]))
  return(invisible(NULL))
}

plot.s2poly <- function(x, ..., add = FALSE){
  if(!add) globe::globeearth(...)
  for(i in seq_along(x$loops)){
    plotloop(x$loops[[i]], ..., sign = x$sign[[i]])
  }
}
