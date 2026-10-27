/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <https://www.gnu.org/licenses/old-licenses/gpl-2.0>.
 */

/** @file four_aspect.h Speed commands for the experimental four-aspect mode. */
#ifndef FOUR_ASPECT_H
#define FOUR_ASPECT_H
#include <cstdint>
#include <limits>

/** Native order: stop, caution, preliminary caution, clear. Speeds are km/h-ish. */
inline constexpr uint16_t FourAspectSpeedLimit(uint8_t aspect)
{
	switch (aspect) {
		case 0: return 0;
		case 1: return 45;
		case 2: return 65;
		default: return std::numeric_limits<uint16_t>::max();
	}
}

/** Latch a permitted signal. Red never constitutes permission to enter. */
inline bool AcceptFourAspectCommand(uint16_t &limit, uint8_t aspect)
{
	if (aspect == 0) return false;
	limit = FourAspectSpeedLimit(aspect);
	return true;
}
#endif /* FOUR_ASPECT_H */
