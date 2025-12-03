<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/fork.h</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#ifndef&nbsp;GENOS_FORK_H<br>
#define&nbsp;GENOS_FORK_H<br>
<br>
#include&nbsp;&lt;cstdint&gt;<br>
#include&nbsp;&lt;igris/compiler.h&gt;<br>
#include&nbsp;&lt;string&gt;<br>
#include&nbsp;&lt;vector&gt;<br>
<br>
namespace&nbsp;genos<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;clone(int&nbsp;(*fn)(void&nbsp;*),&nbsp;void&nbsp;*arg,&nbsp;void&nbsp;*stack,&nbsp;size_t&nbsp;stack_size);<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;execute(const&nbsp;char&nbsp;*cmd);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;int&nbsp;command_process_v(const&nbsp;std::vector&lt;std::string&gt;&nbsp;&amp;argv);<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;int&nbsp;execute(const&nbsp;char&nbsp;*cmd);<br>
&nbsp;&nbsp;&nbsp;&nbsp;//&nbsp;int&nbsp;execute(const&nbsp;std::vector&lt;std::string&gt;&nbsp;&amp;vec);<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;start_command_process_v(const&nbsp;char&nbsp;**argv,&nbsp;size_t&nbsp;stacksize);<br>
<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;waitpid(intptr_t&nbsp;pid);<br>
&nbsp;&nbsp;&nbsp;&nbsp;int&nbsp;waitpid_without_displace(intptr_t&nbsp;pid);<br>
}<br>
<br>
#endif<br>
<!-- END SCAT CODE -->
</body>
</html>
