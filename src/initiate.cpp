#include <vic_R.h>

void popalute_param_state(all_vars_struct *all_vars,
                          soil_con_struct *soil_con,
                          veg_con_struct  *veg_con,
                          lake_con_struct  lake_con,
                          dmy_struct      *dmy_current)
{
    extern option_struct options;

    size_t               Nveg;
    int                  tmp_lake_idx;

    cell_data_struct   **cell;
    energy_bal_struct  **energy;
    lake_var_struct     *lake;
    snow_data_struct   **snow;
    veg_var_struct     **veg_var;

    cell = all_vars->cell;
    energy = all_vars->energy;
    lake = &all_vars->lake_var;
    snow = all_vars->snow;
    veg_var = all_vars->veg_var;

    Nveg = veg_con[0].vegetat_type_num;

    // Initialize all data structures to 0
    initialize_soil(cell, Nveg);
    initialize_snow(snow, Nveg);
    initialize_veg(veg_var, Nveg);
    if (options.LAKES) {
      tmp_lake_idx = lake_con.lake_idx;
      if (tmp_lake_idx < 0) {
        tmp_lake_idx = 0;
      }
      initialize_lake(lake, lake_con, soil_con, &(cell[tmp_lake_idx][0]),
                      false);
    }
    initialize_energy(energy, Nveg);

    // Read initial state from a file if provided
    //if (options.INIT_STATE) {
    //  read_initial_model_state(filep.init_state, all_vars, Nveg,
    //                           options.SNOW_BAND, cellnum, soil_con,
    //                           lake_con);
    //}
    //else {
    //  // else generate a default state
    generate_default_state(all_vars, soil_con, veg_con, dmy_current);
    if (options.LAKES) {
      generate_default_lake_state(all_vars, soil_con, lake_con);
    }
    //}

    // compute those state variables that are derived from the others
    compute_derived_state_vars(all_vars, soil_con, veg_con);
    if (options.LAKES) {
      compute_derived_lake_dimensions(lake, lake_con);
    }
}
