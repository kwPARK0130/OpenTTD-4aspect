/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <https://www.gnu.org/licenses/old-licenses/gpl-2.0>.
 */

/** @file four_aspect.cpp Four-aspect command lifecycle tests. */
#include "../stdafx.h"
#include "../3rdparty/catch2/catch.hpp"
#include "../four_aspect.h"
#include "../safeguards.h"

TEST_CASE("Four aspect: a following train accepts caution and releases it only at a new signal")
{
	uint16_t command = UINT16_MAX;
	REQUIRE(AcceptFourAspectCommand(command, 1));
	CHECK(command == 45);
	// The passed signal changing behind the train cannot replace its command.
	const uint8_t passed_signal_now = 0;
	CHECK_FALSE(AcceptFourAspectCommand(command, passed_signal_now));
	CHECK(command == 45);
	REQUIRE(AcceptFourAspectCommand(command, 2));
	CHECK(command == 65);
	REQUIRE(AcceptFourAspectCommand(command, 3));
	CHECK(command == UINT16_MAX);
}

TEST_CASE("Four aspect: red never grants entry and an approach stop is not an immediate zero-speed cap")
{
	uint16_t command = 65;
	CHECK_FALSE(AcceptFourAspectCommand(command, 0));
	CHECK(command == 65);
	CHECK(FourAspectSpeedLimit(0) == 0);
	CHECK(FourAspectSpeedLimit(1) < FourAspectSpeedLimit(2));
	CHECK(FourAspectSpeedLimit(2) < FourAspectSpeedLimit(3));
	// Extra native aspects supplied by another GRF retain a clear command.
	CHECK(FourAspectSpeedLimit(7) == UINT16_MAX);
}
