##plot for question2.1
dat = read.table("overall.dat")

plot(x = 1:length(dat[,1]), y = dat[,1], type="l", ylab="Number of steps per episode", xlab="episode", col="purple")

##plot for question 2.2
dat = read.table("sums.dat")
x = c(0.0025,0.05,0.1,0.2,0.4,0.5,0.8)
plot(x = x, y = dat[,1], type="l", ylab="Total number of steps", xlab="alpha", col="purple")
