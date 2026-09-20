#include "idk/engine/Engine.hpp"
#include "idk/editor/Editor.hpp"

int main(int argc, char **argv)
{
    (void)argc; (void)argv;

    idk::Engine engine;
    idk::editor::EditorApplication editor;
    engine.run(&editor);

    return 0;
}
