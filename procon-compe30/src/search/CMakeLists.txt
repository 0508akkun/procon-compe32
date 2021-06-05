cmake_minimum_required(VERSION 2.8)
project(procon)
add_subdirectory(Utility)
set(CMAKE_CXX_STANDARD 14)
find_package(Threads REQUIRED)
add_executable(procon
    main.cpp
    AccessServer.cpp
    Battle.cpp
    Action.cpp
    ActionReader.cpp
    Agent.cpp
    Field.cpp
    FieldReader.cpp
    Point.cpp
    ScoreCalculate.cpp
    Team.cpp
    Match.cpp
    MatchReader.cpp
    Human.cpp
    Greedy.cpp
    Act.cpp
    beamsearch.cpp
    Cell.cpp
    Command.cpp
    Player.cpp
    State.cpp
    OptionArg.cpp
    Convolution.cpp
    beamsearch2.cpp
    State2.cpp
    LocalBattle.cpp
    beamsearch3.cpp
    State3.cpp
    beamsearch4.cpp
    State4.cpp
    beamsearch5.cpp
    State5.cpp
    beamsearch6.cpp
    State6.cpp
    beamsearch7.cpp
    State7.cpp
    beamsearch7_2.cpp
    State7_2.cpp
)
target_link_libraries(procon Utility cpprest boost_system ssl crypto Threads::Threads)
