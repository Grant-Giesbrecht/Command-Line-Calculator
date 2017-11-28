//
//  settings_menu.hpp
//  clc
//
//  Created by Grant Giesbrecht on 4/3/17.
//  Copyright © 2017 Grant Giesbrecht. All rights reserved.
//

#ifndef settings_menu_hpp
#define settings_menu_hpp

#include <stdio.h>
#include "interpret_keywords.hpp"
#include "configuration.hpp"
#include "CLIK.hpp"

void run_settings_menu(program_settings& settings);

void show_settings(program_settings);

bool load_settings(std::string filename, program_settings& settings);

bool save_settings(std::string filename, program_settings& settings);

#endif /* settings_menu_hpp */
