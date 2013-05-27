
/*
TODO generate lines / points from gcode input.
*/
function backplot(lines)
{
	function isObject(obj)
	{
		if( Object.prototype.toString.call( obj ) === '[object Object]' )
			return true;
		
		return false;
	}

	lines.forEach(function(line)
	{
		line.forEach(function(word)
		{
			if(isObject(word))
			{
				for(var code in word)
				{
					if(code == g)
					{
						// TODO
					}
				}
			}
		});
	});

	return;
}

