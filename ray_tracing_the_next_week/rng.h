#pragma once

#include <functional>
#include <random>

std::function<float()> rn = std::bind(std::uniform_real_distribution<float>(), std::default_random_engine());
