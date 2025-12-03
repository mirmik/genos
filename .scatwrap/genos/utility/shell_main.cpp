<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/utility/shell_main.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;fcntl.h&gt;<br>
#include&nbsp;&lt;genos/utility/shell.h&gt;<br>
#include&nbsp;&lt;igris/shell/vtermxx.h&gt;<br>
#include&nbsp;&lt;unistd.h&gt;<br>
<br>
void&nbsp;write_callback(const&nbsp;char&nbsp;*str,&nbsp;unsigned&nbsp;int&nbsp;sz)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;ret&nbsp;=&nbsp;write(STDOUT_FILENO,&nbsp;str,&nbsp;sz);<br>
&nbsp;&nbsp;&nbsp;&nbsp;(void)ret;<br>
}<br>
<br>
int&nbsp;shell_main(int&nbsp;argc,&nbsp;char&nbsp;**argv)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;igris::vtermxx&nbsp;rl;<br>
&nbsp;&nbsp;&nbsp;&nbsp;rl.init(48,&nbsp;5);<br>
&nbsp;&nbsp;&nbsp;&nbsp;rl.set_write_callback(write_callback);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;a&nbsp;=&nbsp;open(&quot;/dev/ttyS0&quot;,&nbsp;O_RDONLY);<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;b&nbsp;=&nbsp;open(&quot;/dev/ttyS0&quot;,&nbsp;O_WRONLY);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;char&nbsp;c;<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;while&nbsp;(1)<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;sz&nbsp;=&nbsp;read(STDIN_FILENO,&nbsp;&amp;c,&nbsp;1);<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;rl.newdata(c);<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;&nbsp;&nbsp;&nbsp;}<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
