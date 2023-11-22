#include <evangel/app.h>
#include <stdio.h>

void hook_app_init() { printf("Hello bazel"); }

void hook_app_on_update(){};
void hook_app_on_render(){};
void hook_app_free(){};