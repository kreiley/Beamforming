reset
set angles degrees
set polar
set grid polar 30 lc rgbcolor "#999999"
unset border
unset param
set size ratio 1 1,1
set xtics axis nomirror -50,10
unset ytics
unset key
set style data line
set xrange[-50:50]
set yrange[-50:50]
set rrange[-50:0]
set label 1 "0°" at graph 1.01,0.5 front
set label 2 "180°" at graph -0.01,0.5 right front
set label 3 "-90°" at graph 0.5,-0.03 center front
set label 4 "90°" at graph 0.5,1.03 center front
plot 'beamPattern.dat' u 2:5
