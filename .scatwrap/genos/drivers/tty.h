<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/drivers/tty.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_DRIVERS_TTY_H<br>
#define&nbsp;GENOS_DRIVERS_TTY_H<br>
<br>
#include&nbsp;&lt;genos/chardev.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;tty_driver;<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;tty_linedisc;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;tty&nbsp;:&nbsp;public&nbsp;genos::chardev<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tty_driver&nbsp;*driver&nbsp;=&nbsp;nullptr;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tty_linedisc&nbsp;*linedisc&nbsp;=&nbsp;nullptr;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;igris::dlist_base&nbsp;rx_wait&nbsp;=&nbsp;{};<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pid_t&nbsp;gid&nbsp;=&nbsp;0;&nbsp;//&nbsp;holder&nbsp;group&nbsp;id<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;pid_t&nbsp;pid&nbsp;=&nbsp;0;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tty();<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;tty(const&nbsp;char&nbsp;*name,&nbsp;tty_driver&nbsp;*drv,&nbsp;tty_linedisc&nbsp;*ldisc);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;write(const&nbsp;void&nbsp;*data,&nbsp;unsigned&nbsp;int&nbsp;size)&nbsp;override;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;read(void&nbsp;*data,&nbsp;unsigned&nbsp;int&nbsp;size)&nbsp;override;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;receive_newchar(char&nbsp;c);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;send_signal(int&nbsp;sig);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;on_open()&nbsp;override;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;on_release()&nbsp;override;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;size_t&nbsp;send_over_driver(const&nbsp;char&nbsp;*data,&nbsp;size_t&nbsp;size);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;size_t&nbsp;send_over_driver(char&nbsp;c)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;send_over_driver(&amp;c,&nbsp;1);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;notify_readers();<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;void&nbsp;signal(int&nbsp;sig);<br>
&nbsp;&nbsp;&nbsp;&nbsp;};<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
