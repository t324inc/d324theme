/*
 |--------------------------------------------------------------------------
 | Mix Asset Management
 |--------------------------------------------------------------------------
 |
 | Mix provides a clean, fluent API for defining some Webpack build steps
 | for your application. See https://github.com/JeffreyWay/laravel-mix.
 |
 */

const proxy = 'http://d324.local';
const fs = require('fs');
const yaml = require('js-yaml');
const mix = require('laravel-mix');
const ExtraWatchWebpackPlugin = require( 'extra-watch-webpack-plugin');

/*
 |--------------------------------------------------------------------------
 | Propagate theme settings
 |--------------------------------------------------------------------------
 */

// Read in YAML file containing D324 theme settings
const confYaml = yaml.safeLoad(fs.readFileSync('./d324theme.settings.yml', 'utf8'));

// Create SASS vars to inject into SCSS
const sassInjection = prepSassVars(confYaml);
const sassMsg = "// DON'T UPDATE THIS FILE\n"
  + "// Instead, update the file d324theme.settings.yml and run "
  + "\"npm run dev\" or \"npm run production\".\n\n";
fs.writeFileSync('src/sass/base/_d324_variables.scss',
  sassMsg + sassInjection);

// Create Gutenberg vars for theme support
const gutenVars = prepGutenbergVars(confYaml);
const gutenMsg = "# Don't update this file unless you want Gutenberg colors "
  + "to become separated from D324 theme variables.  Instead,\n"
  + "# update the file d324theme.settings.yml and run \"npm run "
  + "dev\" or \"npm run production\".\n\n";
fs.writeFileSync('d324theme.gutenberg.yml',
  gutenMsg + gutenVars);

/*
 |--------------------------------------------------------------------------
 | Configuration
 |--------------------------------------------------------------------------
 */

mix
  .setPublicPath('assets')
  .disableNotifications()
  .options({
    processCssUrls: false
  })
  .sourceMaps(false, 'source-map')
  .webpackConfig({
    plugins: [
      new ExtraWatchWebpackPlugin({
        files: [ 'd324theme.settings.yml' ],
      }),
    ],
  });

/*
 |--------------------------------------------------------------------------
 | Browsersync
 |--------------------------------------------------------------------------
 */

mix.browserSync({
  proxy: proxy,
  files: ['assets/js/**/*.js', 'assets/css/**/*.css'],
  stream: true,
});

/*
 |--------------------------------------------------------------------------
 | SASS
 |--------------------------------------------------------------------------
 */

mix.sass('src/sass/d324theme.style.scss', 'css')
  .sass('src/sass/d324theme.gutenberg.scss', 'css');

/*
 |--------------------------------------------------------------------------
 | JS
 |--------------------------------------------------------------------------
 */

mix.js('src/js/d324theme.script.js', 'js');

/*
 |--------------------------------------------------------------------------
 | Functions
 |--------------------------------------------------------------------------
 */

function prepSassVars(confYaml) {
  const sections = [
    'd324colors',
    'd324fontfamilies',
    'd324layout',
    'd324bootstrapoptions'
  ];
  return sections.map(function(section) {
    return Object.keys(confYaml[section]).map(function (name) {
      return "$" + name + ": " + confYaml[section][name].value + ";";
    }).join('\n');
  }).join('\n\n');
}

function prepGutenbergVars(confYaml) {
  let gutenberg_vars = {
    'theme-support': {
      colors: []
    }
  };
  Object.keys(confYaml['d324colors']).map(function (name) {
    // Create Gutenberg theme swatch vars
    gutenberg_vars['theme-support'].colors.push({
      slug: name,
      name: confYaml['d324colors'][name].name,
      color: confYaml['d324colors'][name].value
    });
  });
  return yaml.dump(gutenberg_vars);
}
