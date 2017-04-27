#include <kernel/resources/SchedeeResource.h>
#include <kernel/sched/Schedee.h>

void Genos::SchedeeResource::setParent(Schedee* parent) {
	if (parent) parent->addResource(this);
	else this->parent = nullptr; 
}

void Genos::SchedeeResource::unbindFromParent() {
	if (parent) parent->unbindResource(this);
}