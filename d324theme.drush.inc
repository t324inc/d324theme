<?php
/**
 * @file
 * Contains Drush hooks.
 */

/**
 * Implements hook_drush_command().
 */
function d324theme_drush_command() {
  $items = array();

  $items['d324theme'] = array(
    'description' => 'Create a d324theme subtheme.',
    'arguments' => array(
      'name' => 'The name of your subtheme.',
    ),
    'options' => array(
      'machine_name' => 'The machine-readable name of your subtheme. This will be auto-generated from the human-readable name if ommited.',
      'description' => 'The description of your subtheme',
      'destination' => 'The destination of your subtheme. Defaults to "./themes/d324".',
    ),
    'examples' => array(
      'drush d324theme "My Theme"' => 'Creates a d324theme subtheme called "My Theme", using the default options.',
      'drush d324theme "My Theme" --machine_name=my_theme' => 'Creates a d324theme subtheme called "My Theme" with a specific machine name.',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function d324theme_drush_help($section) {
  switch ($section) {
    case 'drush:d324theme':
      return dt('This command will create a d324theme subtheme. See examples to get started.');
      break;
  }
}

/**
 * Implements drush_hook_COMMAND().
 */
function drush_d324theme($name = NULL, $machine_name = NULL) {

  // If no $name provided, abort.
  if (!$name) {
    drush_print(dt('Theme name missing. See help using drush d324theme --help.'));
    return;
  }

  // Determine the theme name.
  if (!isset($name)) {
    $name = drush_get_option('name');
  }

  // Determine the machine name.
  if (!isset($machine_name)) {
    $machine_name = drush_get_option('machine_name');
  }
  if (!$machine_name) {
    $machine_name = $name;
  }
  $machine_name = str_replace(' ', '_', strtolower($machine_name));
  $search = array(
    '/[^a-z0-9_]/', // Remove characters not valid in function names.
    '/^[^a-z]+/',   // Functions must begin with an alpha character.
  );
  $machine_name = preg_replace($search, '', $machine_name);

  // Description of subtheme.
  $description = (drush_get_option('description')) ? trim(drush_get_option('description')) : 'A theme based on D324Theme.';

  // Determine the path to the new subtheme.
  $subtheme_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/themes/d324/' . $machine_name);

  // Make a fresh copy of the kit.
  $kit_path = drush_normalize_path(drush_get_context('DRUSH_DRUPAL_ROOT') . '/' . drupal_get_path('theme', 'd324theme') . '/SUBTHEME_KIT');

  if (!is_dir(dirname($subtheme_path))) {
    drush_die(dt('The directory "!directory" was not found.', array('!directory' => dirname($subtheme_path))));
  }
  drush_op('drush_copy_dir', $kit_path, $subtheme_path);

  // Alter the contents of the .info file based on the command options.
  $alterations = array(
    'D324_SUBTHEME_NAME' => $name,
    'D324_SUBTHEME_DESCRIPTION' => $description,
    'D324_SUBTHEME_MACHINE_NAME' => $machine_name,
    'hidden: true' => '',
  );

  $subtheme_kit_name = 'd324_subtheme';

  // Replace all occurrences of '{{machine_name}}' with the machine name of our sub theme.
  $files_to_replace = d324theme_get_files_to_make_replacements($subtheme_kit_name);
  foreach ($files_to_replace as $file_to_replace) {
    drush_op('d324theme_file_str_replace', $subtheme_path . '/' . $file_to_replace, array_keys($alterations), $alterations);
  }

  // Rename files.
  $files_to_rename = array(
    '{{subtheme_kit_name}}.info.yml',
    '{{subtheme_kit_name}}.libraries.yml',
    '{{subtheme_kit_name}}.breakpoints.yml',
    '{{subtheme_kit_name}}.theme',
    'config/schema/{{subtheme_kit_name}}.schema.yml',
    'src/sass/{{subtheme_kit_name}}.style.scss',
    'src/js/{{subtheme_kit_name}}.script.js',
  );
  foreach ($files_to_rename as $file_to_rename_path) {
    $file_original_path = $subtheme_path . '/' . str_replace('{{kit}}', $kit, $file_to_rename_path);
    $file_new_path = $subtheme_path . '/' . str_replace('{{kit}}', $machine_name, $file_to_rename_path);
    drush_op('rename', drush_normalize_path($file_original_path), drush_normalize_path($file_new_path));
  }

  // Notify user of the newly created theme.
  $message = 'Successfully created the D324 subtheme "!name" in "!path" using the "!kit" kit';

  $message = dt($message . '.', array(
    '!name' => $name,
    '!path' => $subtheme_path,
    '!kit' => $kit,
  ));
  drush_print($message);
}

/**
 * Replace strings in a file.
 */
function d324theme_file_str_replace($file_path, $find, $replace) {
  $file_path = drush_normalize_path($file_path);
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}

/**
 * Returns an array of files to make string replacements.
 */
function d324theme_get_files_to_make_replacements($subtheme_kit_name = 'd324_subtheme') {
  return array(
    $subtheme_kit_name . '.info.yml',
    $subtheme_kit_name . '.libraries.yml',
    $subtheme_kit_name . '.theme',
    'package.json',
    'package-lock.json',
    'webpack.mix.js',
    'README.md',
    'templates/content/node.html.twig',
    'config/schema/' . $subtheme_kit_name . '.schema.yml',
  );
}

