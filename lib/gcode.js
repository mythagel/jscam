GCODE = {};

GCODE.toString = function(lines)
{
	var s = "";

	function fixedPoint(v)
	{
		return parseFloat(v.toFixed(6));
	};

	lines.forEach(function(line)
	{
		line.forEach(function(word)
		{
			if(isObject(word))
			{
				for(var code in word)
				{
					if(code == "comment")
						s += " (" + word[code] + ")";
					else
						s += code + fixedPoint(word[code]);
				}
				s += " ";
			}
			else
			{
				// Comment is only allowed at end of line.
				s += "; " + word;
			}
		});
		s += "\n";
	});

	return s;
}
