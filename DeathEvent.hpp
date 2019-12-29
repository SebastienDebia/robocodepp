/**
 * Copyright (c) 2001-2019 Mathew A. Nelson and Robocode contributors
 * All rights reserved. This program and the accompanying materials
 * are made available under the terms of the Eclipse License v1.0
 * which accompanies this distribution, and is available at
 * https://robocode.sourceforge.io/license/epl-v10.html
 */
#pragma once

#include "Event.hpp"

/**
 * This event is sent to {@link Robot#onDeath(DeathEvent) onDeath()} when your
 * robot dies.
 *
 * @author Mathew A. Nelson (original)
 * @author Flemming N. Larsen (contributor)
 */
class DeathEvent : public Event
{
public:

	/**
	 * Called by the game to create a new DeathEvent.
	 */
	DeathEvent()
	{
	}
};
