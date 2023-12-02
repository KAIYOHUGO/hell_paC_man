# Hell Pac Men

[![wakatime](https://wakatime.com/badge/user/018b677c-491b-487f-843a-068f3eb4badc/project/018bb6a2-2c88-476b-9a8c-5d3a3cfd7f48.svg)](https://wakatime.com/badge/user/018b677c-491b-487f-843a-068f3eb4badc/project/018bb6a2-2c88-476b-9a8c-5d3a3cfd7f48)

> Hell is not bad but you know heaven exist.
> 
> C & Cmake is not bad but you know other language exist.

# Code Design

- `V{Structure Name}` is a Trait (VTable)
- `P{Structure Name}` is a Pointer of VTable (`self*, VTable*`)
- `C{Structure Name}` is a Collection of method of `{Structure Name}`
- `hook_{Function Name}` is a Hook call by engine

# Naming

- `StructureUpperCamelCase`
- `EnumUpperCamelCase`
- `EnumUpperCamelCase_VariantUpperCamelCase`
- `ItemMacroUpperCamelCase`
- `function_snake_case`
- `Type(FakeGeneric)`
- `fake_typed_function_macro_snake_case`
- `OverLoadingMacroCamelCase`

# References

- [Mojoc (C)](https://github.com/scottcgi/Mojoc)
- [Bevy (Rust)](https://bevyengine.org/)
- [Flecs (C)](https://github.com/SanderMertens/flecs)
- [ECS FAQ](https://github.com/SanderMertens/ecs-faq)
- [Let's build an Entity Component System (Zig)](https://devlog.hexops.com/2022/lets-build-ecs-part-2-databases/)
- [Building an ECS #2: Archetypes and Vectorization (C)](https://ajmmertens.medium.com/building-an-ecs-2-archetypes-and-vectorization-fe21690805f9)
- [Google Test (C++)](http://google.github.io/googletest/)


[DOC](https://docs.google.com/document/d/1pbmqKgjQ75Juk1BeeBKcBVxS9AebBml3H2rgWHhogiw/edit)