dnl Graphviz package version number, (as distinct from shared library version)
dnl For the minor number: odd => unstable series
dnl                       even => stable series
dnl For the micro number: 0 => in-progress development
dnl                       timestamp => tar-file snapshot or release
m4_define(graphviz_version_major, 2)
m4_define(graphviz_version_minor, 40)
m4_define(graphviz_version_micro, 1)
dnl NB: the next line gets changed to a date/time string for development releases
dnl m4_define(graphviz_version_micro, 20161225.0304)
m4_define(graphviz_version_date, 20161225.0304)
m4_define(graphviz_collection, development)
m4_define(graphviz_version_commit, 67cd2e5)
