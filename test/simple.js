init();

begin_block("setup");
	feed_rate(100);
	spindle_on(300);
	//load_tool(5);
	//tool_change(5);
	optional_pause("Post Setup");
end_block();


