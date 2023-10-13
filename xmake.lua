add_rules("mode.debug", "mode.release")

add_requires("stb")

target("rt_in_one_weekend")
    set_kind("binary")
    add_files("src/*.cpp")
    add_packages("stb")
    set_languages("c++20")