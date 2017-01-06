#include "stdio.h"
#include "smach.h"

#include <smachL.h>

struct smach_s smach;

char buf[11] = "HelloWorld";
char buf2[6] = "Mirmik";

int main() {
	smachL_init(&smach);
	smachL_pushString(&smach, "HelloWorld");
	smachL_pushString(&smach, "HelloWorld");
	smach_pushBack(&smach, smach_constructPlaced(&smach, buf2, 5));
	//smach_popBack(&smach);
	smachL_pushInt32(&smach, 34);
	
	smach_popAll(&smach);
	smachL_debugInfo(&smach);
	printf("%s\r\n", smachL_toString(&smach, 2, NULL));

}