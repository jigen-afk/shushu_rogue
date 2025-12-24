cmake_minimum_required(VERSION 3.10)
project(shushurogue)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Windows下设置控制台编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

if(MINGW)
    add_compile_options(-finput-charset=UTF-8 -fexec-charset=UTF-8)
endif()

set(SOURCES
    main.cpp
    Player.cpp
    Monster.cpp
    Card.cpp
    Buff.cpp
    Relic.cpp
    CombatSystem.cpp
    CombatDeck.cpp
    EventSystem.cpp
    MapSystem.cpp
    ShopSystem.cpp
    RandomGenerator.cpp
)

set(HEADERS
    Player.h
    Monster.h
    Card.h
    Buff.h
    Relic.h
    CombatSystem.h
    CombatDeck.h
    EventSystem.h
    MapSystem.h
    ShopSystem.h
    RandomGenerator.h
    Register.h
    Creature.h
    Effect.h
    Enum.h
    AttackEffectAdapter.h
    BlockEffectAdapter.h
    SecondWindEffectAdapter.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})