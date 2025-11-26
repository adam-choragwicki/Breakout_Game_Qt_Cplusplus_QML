set(SOURCE_FILES
        # ───────────────────────────────
        # Core Game Entry / Root
        # ───────────────────────────────
        game.cpp game.h
        controller.cpp controller.h
        model.cpp model.h
        game_loop.cpp game_loop.h
        input_handler.cpp input_handler.h
        resolution.h
        error_utils.h
        log_manager.h
        game_coordinator.cpp game_coordinator.h
        state_machine/i_state_actions.h
        log_manager.cpp

        # ───────────────────────────────
        # Configuration
        # ───────────────────────────────
        config/game_config.cpp config/game_config.h

        # ───────────────────────────────
        # Entities
        # ───────────────────────────────
        entities/paddle.h
        entities/ball.h
        entities/brick.h

        # ───────────────────────────────
        # Brick system
        # ───────────────────────────────
        bricks_manager.h
        bricks_model.cpp bricks_model.h
        brick_layouts/brick_layout_library.cpp
        brick_layouts/brick_layout_library.h

        # ───────────────────────────────
        # Asset management
        # ───────────────────────────────
        asset_management/asset_loader.cpp
        asset_management/asset_loader.h
        asset_management/asset_repository.h

        # ───────────────────────────────
        # Collision detection
        # ───────────────────────────────
        collision_common.h
        collision_detection/collision_detection_utils.h
        collision_detection/paddle_collision_detector.cpp
        collision_detection/paddle_collision_detector.h
        collision_detection/arena_edge_collision_detector.cpp
        collision_detection/arena_edge_collision_detector.h
        collision_detection/brick_collision_detector.cpp
        collision_detection/brick_collision_detector.h

        # ───────────────────────────────
        # Collision handling
        # ───────────────────────────────
        collision_handling/arena_edge_collision_handler.h
        collision_handling/paddle_collision_handler.h
        collision_handling/brick_collision_handler.h

        # ───────────────────────────────
        # State machine
        # ───────────────────────────────
        state_machine/game_state_type.h
        state_machine/game_state_manager.h
        state_machine/abstract_state.h
        state_machine/states/initialization_state.h
        state_machine/states/ready_to_start_state.h
        state_machine/states/running_state.h
        state_machine/states/escape_menu_state.h
        state_machine/states/stopped_win_state.h
        state_machine/states/stopped_lose_state.h
        state_machine/states/paused_for_debug_state.h

        # ───────────────────────────────
        # Audio / Visual
        # ───────────────────────────────
        sound_player.cpp sound_player.h
        overlay_manager.cpp overlay_manager.h
        window_manager.cpp window_manager.h
        qml_helper.h

        # ───────────────────────────────
        # Managers
        # ───────────────────────────────
        ball_manager.cpp ball_manager.h
)

add_library(${PROJECT_NAME}Backend ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME}Backend PUBLIC ${PROJECT_SOURCE_DIR}/src)
target_link_libraries(${PROJECT_NAME}Backend PUBLIC Qt6::Core Qt6::Quick Qt6::Multimedia)

target_compile_definitions(${PROJECT_NAME}Backend PRIVATE
        APP_VERSION="${PROJECT_VERSION}"
        BUILD_TYPE="$<CONFIG>"
)

qt_add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Backend Qt6::Quick BreakoutQmlLibplugin)

qt_add_resources(${PROJECT_NAME}
        PREFIX /
        FILES "../assets/resources.qrc"
)

if (WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE $<CONFIG:Release>) # this avoids opening console window on Windows in Release mode
endif ()

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin) # Set the output directory for the executable
