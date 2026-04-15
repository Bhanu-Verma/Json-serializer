#include <iostream>
#include <vector>
#include <string>
#include <cassert>
#include "json_lib.hpp"
#include "parser.hpp"
#include "tester/my_test_lib.hpp"

using namespace std::string_literals;

int main() {
    try {
        std::string neuralNet = R"(
            {
                "Layers": [
                    {
                    "activationFunction": 2,
                    "bias": [0.0, 3.0189532406755846],
                    "dropOut": 0.0,
                    "inputSize": 4,
                    "outputSize": 2,
                    "weights": [
                        [
                        -0.08346827309618231, -0.015846791948910366, -0.08349304374739283,
                        0.09609414523049431
                        ],
                        [
                        0.4561129542902446, 0.9538394630885084, -2.825330296664123,
                        -2.334613611419187
                        ]
                    ]
                    },
                    {
                    "activationFunction": 2,
                    "bias": [
                        -2.2411061503791168e-7, -3.1034365622270295e-7, -0.0071385247919979146,
                        -0.05938659439333594, -0.0042402320760852
                    ],
                    "dropOut": 0.0,
                    "inputSize": 2,
                    "outputSize": 5,
                    "weights": [
                        [0.07761276375946172, -0.009657275184744879],
                        [-0.05583640137434418, -0.020520167348293886],
                        [0.0598274549951375, 3.0918124876854023],
                        [0.019612654606685084, 1.0444876714184785],
                        [-0.05842645281322193, 2.34373381525731]
                    ]
                    },
                    {
                    "activationFunction": 4,
                    "bias": [-4.179055764792003, 1.96682621188172, 5.249449168299232],
                    "dropOut": 0.0,
                    "inputSize": 5,
                    "outputSize": 3,
                    "weights": [
                        [
                        -0.058839361149710286, -0.08434826150741549, 1.993527038790463,
                        0.7667121920923895, 1.4707015926604516
                        ],
                        [
                        -0.000982528760024976, 0.029823991648432937, 1.3844680098786148,
                        0.37708103391606607, 1.1566501465547734
                        ],
                        [
                        0.0693492906978207, 0.08485103545013141, -1.8879924350818913,
                        -0.5986392654413258, -1.3897169174049828
                        ]
                    ]
                    }
                ],
                "inputDim": 4,
                "numberOfLayers": 3
                }
        )";

        json::Json nn = json::parse(neuralNet);
        std::cout << nn << '\n';
        // MyTester::runAllTests();
    } catch (std::string_view e) {
        std::cerr << e << "\n";
        return 1;
    }

    // MyTester::runAllTests();
    return 0;
}