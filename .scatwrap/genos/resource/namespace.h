<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/resource/namespace.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_RESOURCE_NAMESPACE_H<br>
#define&nbsp;GENOS_RESOURCE_NAMESPACE_H<br>
<br>
#include&nbsp;&lt;genos/resource/file_head.h&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
#include&nbsp;&lt;igris/container/dlist.h&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;class&nbsp;namespace_manager<br>
&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;const&nbsp;char&nbsp;*_path;<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;igris::dlist_node&nbsp;namespace_list_lnk&nbsp;=&nbsp;{};<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;public:<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;namespace_manager(const&nbsp;char&nbsp;*zone);<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;~namespace_manager();<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;virtual&nbsp;int&nbsp;lookup(file_head&nbsp;**filpp,&nbsp;const&nbsp;char&nbsp;*path)&nbsp;=&nbsp;0;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;const&nbsp;char&nbsp;*path()<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;{<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;_path;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;}<br>
&nbsp;&nbsp;&nbsp;&nbsp;};<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;extern&nbsp;igris::dlist&lt;namespace_manager,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&amp;namespace_manager::namespace_list_lnk&gt;<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;namespace_list;<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
