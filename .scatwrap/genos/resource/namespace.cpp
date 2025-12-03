<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/resource/namespace.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;genos/resource/namespace.h&gt;<br>
#include&nbsp;&lt;igris/container/dlist.h&gt;<br>
#include&nbsp;&lt;string.h&gt;<br>
<br>
INIT_PRIORITY(140)<br>
igris::dlist&lt;genos::namespace_manager,<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&amp;genos::namespace_manager::namespace_list_lnk&gt;<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::namespace_list;<br>
<br>
genos::namespace_manager::namespace_manager(const&nbsp;char&nbsp;*path)<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;_path&nbsp;=&nbsp;path;<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::namespace_list.move_back(*this);<br>
}<br>
<br>
genos::namespace_manager::~namespace_manager()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::namespace_list.pop(*this);<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
