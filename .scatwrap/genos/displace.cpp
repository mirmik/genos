<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>genos/displace.cpp</title>
</head>
<body>
<!-- BEGIN SCAT CODE -->
#include&nbsp;&lt;asm/irq.h&gt;<br>
#include&nbsp;&lt;asm/startup.h&gt;<br>
#include&nbsp;&lt;genos/displace.h&gt;<br>
#include&nbsp;&lt;genos/schedee.h&gt;<br>
#include&nbsp;&lt;igris/sync/syslock.h&gt;<br>
<br>
__attribute__((deprecated))&nbsp;int&nbsp;__displace__()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;genos::schedee&nbsp;*sch&nbsp;=&nbsp;genos::current_schedee();<br>
&nbsp;&nbsp;&nbsp;&nbsp;sch-&gt;syslock_counter_save&nbsp;=&nbsp;syslock_counter();<br>
&nbsp;&nbsp;&nbsp;&nbsp;return&nbsp;sch-&gt;displace();<br>
}<br>
<br>
__attribute__((weak))&nbsp;void&nbsp;__schedule__()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;while&nbsp;(1)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;;<br>
}<br>
<br>
__attribute__((weak))&nbsp;void&nbsp;__context_displace_vector__()<br>
{<br>
&nbsp;&nbsp;&nbsp;&nbsp;RESET_STACK();<br>
&nbsp;&nbsp;&nbsp;&nbsp;irqs_enable();<br>
&nbsp;&nbsp;&nbsp;&nbsp;syslock_reset();<br>
&nbsp;&nbsp;&nbsp;&nbsp;while&nbsp;(1)<br>
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;__schedule__();<br>
}<br>
<!-- END SCAT CODE -->
</body>
</html>
