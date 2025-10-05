add_library(Application_lib STATIC 
    app.cpp 
    parser.cpp
)
target_link_libraries(Application_lib PUBLIC GameScene_lib)