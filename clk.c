Starting from DTS point:

CLK_OF_DECLARE(at91rm9200_clk_main_osc, "atmel,at91rm9200-clk-main-osc",
	       of_at91rm9200_clk_main_osc_setup);


of_at91sam9260_clk_slow_setup()
- at91_clk_register_sam9260_slow(regmap, name, parent_name, bypass);		slowck+clk_hw is born!
 - clk_hw_register(NULL, slowck->hw);
  - clk_register(dev, hw)     returns clk*									clk_core is born!
   - INIT_HLIST_HEAD(&core->clks);											create clk list for core?
   - __clk_create_clk(hw, NULL, NULL);										clk is born!
    - hlist_add_head(&clk->clks_node, &hw->core->clks);						clk is added as a node to the hlist core->clks
   - __clk_core_init(core);
- of_clk_add_hw_provider(np, of_clk_hw_simple_get, hw);						of_clk_provider is born! But we never see the message!




__clk_create_clk creates clk from clk_hw (in memory)


struct clk *__clk_create_clk(struct clk_hw *hw, const char *dev_id,
			     const char *con_id)


