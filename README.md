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
-`EnumUpperCamelCase_VariantUpperCamelCase`
- `ItemMacroUpperCamelCase`
- `function_snake_case`
- `Type(FakeGeneric)`
- `fake_typed_function_macro_snake_case`
