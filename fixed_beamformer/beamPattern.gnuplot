reset
unset key
set xlabel "arrival Angle (degrees)" font "arial, 12"
set ylabel "gain(dB)" font "arial,12"
set grid lc rgbcolor "#BBBBBB"
plot 'beamPattern.dat' u 2:5 w l
