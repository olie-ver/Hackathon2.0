#include "Core.hpp"
#include "Runner.hpp"
#include "Renderer.hpp"

#include <iostream>

using namespace std;

namespace internal {
    int main(int argc, char** argv) {
        if (argc != 1) {
            cerr << "Usage: ./runTests" << endl;
            return EXIT_FAILURE;
        }

        Core::TestRun run;
        Runner::runAllRegisteredTests(run);
        Renderer::Renderer renderer;

        renderer.render(run);
    }
}