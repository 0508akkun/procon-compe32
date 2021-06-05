TARGET=$1

MainBuild() {
    g++ main.cpp \
    Battle.cpp \
    Action.cpp ActionReader.cpp \
    Agent.cpp \
    Field.cpp FieldReader.cpp \
    Point.cpp \
    ScoreCalculate.cpp \
    Team.cpp \
    Match.cpp MatchReader.cpp \
    Utility/Array.cpp \
    Utility/Color.cpp \
    AccessServer.cpp \
    Human.cpp \
    Act.cpp beamsearch.cpp Cell.cpp Command.cpp Player.cpp State.cpp \
    Greedy.cpp \
    OptionArg.cpp \
    -std=c++14 -lcpprest -lboost_system -lssl -lcrypto -pthread \
    -o procon
}

TestBuild () {
    g++ test.cpp Action.cpp FieldReader.cpp Field.cpp Match.cpp Agent.cpp Team.cpp \
    Act.cpp beamsearch.cpp Cell.cpp Command.cpp Player.cpp Point.cpp ScoreCalculate.cpp \
    State.cpp \
    Convolution.cpp \
    -std=c++14 -o test
}

ReleaseBuild() {
    cd release
    make
    cd ..
    cp release/procon ./procon
}

case ${TARGET} in
    "-t") TestBuild
        ;;
    "--test") TestBuild
        ;;
    "-m") MainBuild
        ;;
    "--main") MainBuild
        ;;
    "-r") ReleaseBuild
        ;;
    "--release") ReleaseBuild
        ;;
    *) MainBuild
        ;;
esac
