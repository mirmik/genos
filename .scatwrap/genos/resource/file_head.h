<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/resource/file_head.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_RESOURCE_FILE_HEAD_H<br>
#define&nbsp;GENOS_RESOURCE_FILE_HEAD_H<br>
<br>
#include&nbsp;&lt;fcntl.h&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
#include&nbsp;&lt;stdint.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;file_head<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;uint8_t&nbsp;refs;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;uint8_t&nbsp;flags;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;bool&nbsp;is_open()<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;refs;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;write(const&nbsp;void&nbsp;*data,&nbsp;unsigned&nbsp;int&nbsp;size)&nbsp;=&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;read(void&nbsp;*read,&nbsp;unsigned&nbsp;int&nbsp;size)&nbsp;=&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;on_open()&nbsp;=&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;on_release()&nbsp;=&nbsp;0;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;wait_for_avail()<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char&nbsp;getc()<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;char&nbsp;c;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;read(&amp;c,&nbsp;1);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;c;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;};<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
