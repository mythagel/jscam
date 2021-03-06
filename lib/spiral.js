
/*
UNTESTED.
Changing the parameter a will turn the spiral, while b controls the distance between successive turnings.
 steps: #of steps per turn
 loops: number of loops (can be fractional, i.e. 0.5)
 start_theta: angle where the spiral starts
*/
function spiral(machine, center, steps, loops, a, b, start_theta)
{
	var step = 2 * Math.PI / steps;
	var end = 2 * Math.PI * loops;
	
	for (var angle = 0; angle < end; angle += step)
	{
		var pos = (a + b * (angle + start_theta));
		x = center.x + pos * Math.cos(angle);
		y = center.y + pos * Math.sin(angle);

		machine.linear({x: x}, {y: y});
	}
}

