#pragma once

#include <memory>

class Object;
class Cell;

std::shared_ptr<Object> QuoteDatum(std::shared_ptr<Cell> quote_list);
