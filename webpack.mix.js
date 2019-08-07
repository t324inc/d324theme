/*
 |--------------------------------------------------------------------------
 | Mix Asset Management
 |--------------------------------------------------------------------------
 |
 | Mix provides a clean, fluent API for defining some Webpack build steps
 | for your application. See https://github.com/JeffreyWay/laravel-mix.
 |
 */

const fs = require('fs');
const yaml = require('js-yaml');
const mix = require('laravel-mix');
const ExtraWatchWebpackPlugin = require( 'extra-watch-webpack-plugin');

// Read in YAML file containing D324 theme settings
const confYaml = yaml.safeLoad(fs.readFileSync('./d324theme.settings.yml', 'utf8'));

const utilitySettings = confYaml['d324utilityoptions'];

const proxy = utilitySettings['browsersync-proxy-url'].value;

/*
 |--------------------------------------------------------------------------
 | Propagate theme settings
 |--------------------------------------------------------------------------
 */

// Create SASS vars to inject into SCSS
const sassInjection = prepSassVars(confYaml);

const sassMsg = "// DON'T UPDATE THIS FILE\n"
  + "// Instead, update the file d324theme.settings.yml and run \"npm run dev\" or \"npm run production\"\n"
  + "// This file is automatically regenerated each time \"npm run dev\" or equivalent build process is executed, so \n"
  + "// DO NOT CHANGE THESE VARIABLES HERE... They'll be overwritten later.\n"
  + "// Change them inside of d324theme.settings.yml \"npm run dev\" or \"npm run production\".\n\n";
fs.writeFileSync('src/sass/base/_d324theme_settings.scss',
  sassMsg + sassInjection);

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

mix.sass('src/sass/d324theme.bootstrap.scss', 'css')
  .sass('src/sass/d324theme.style.scss', 'css');

/*
 |--------------------------------------------------------------------------
 | JS
 |--------------------------------------------------------------------------
 */

mix.js('src/js/d324theme.script.js', 'js')
  .js('src/js/d324theme.breakoutfix.js', 'js');

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
  const sassVars = sections.map(function(section) {
    return Object.keys(confYaml[section]).map(function (name) {
      return "$" + name + ": " + confYaml[section][name].value + ";";
    }).join('\n');
  }).join('\n\n');
  const fontBase = Object.keys(confYaml['d324themefontsizing']['body']).map(function (name) {
    return "$d324-base-" + name + ": " + confYaml['d324themefontsizing']['body'][name] + ";";
  }).join('\n');
  const headingsBase = Object.keys(confYaml['d324themefontsizing']['headings']).map(function (elem) {
    return Object.keys(confYaml['d324themefontsizing']['headings'][elem]).map(function (name) {
      return "$d324-" + elem + "-" + name + ": " + confYaml['d324themefontsizing']['headings'][elem][name] + ";";
    }).join('\n');
  }).join('\n\n');
  return [sassVars, fontBase, headingsBase].join('\n\n');
}
