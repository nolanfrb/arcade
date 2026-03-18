/* ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 * EPITECH PROJECT - Thu, Mar, 2026 * Title           - arcade * Description - *
 *     Text *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 * *
 *                                                                                      *
 *       _|_|_|_|  _|_|_|    _|_|_|  _|_|_|_|_|  _|_|_|_|    _|_|_|  _|    _| *
 *       _|        _|    _|    _|        _|      _|        _|        _|    _| *
 *       _|_|_|    _|_|_|      _|        _|      _|_|_|    _|        _|_|_|_| *
 *       _|        _|          _|        _|      _|        _|        _|    _| *
 *       _|_|_|_|  _|        _|_|_|      _|      _|_|_|_|    _|_|_|  _|    _| *
 *                                                                                      *
 * ------------------------------------------------------------------------------------
 */

#ifndef INCLUDED_TEXT_HPP
#define INCLUDED_TEXT_HPP

#include <array>
#include <cstdint>
#include <string>
#include "Position.hpp"

struct Text {
  Position position;
  std::string text;
  std::array<uint8_t, 4> color;
};

#endif
