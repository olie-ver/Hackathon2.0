#include "../include/Core.hpp"
#include "../include/Runner.hpp"
#include "../include/Renderer.hpp"

#include <iostream>

using namespace std;
using namespace internal;

int main(int argc, char** argv) {
    if (argc != 1) {
        cerr << "Usage: ./runTests" << endl;
        return EXIT_FAILURE;
    }

    Core::TestRun run;
    Runner::runAllRegisteredTests(run);
    Renderer::Renderer renderer;

    renderer.render(run);
    return EXIT_SUCCESS;
}