<?php
/**
 * @file
 * Theme and preprocess functions for forms.
 */

use Drupal\Core\Form\FormStateInterface;

/**
 * Implements hook_theme_suggestions_HOOK_alter().
 */

/**
 * Implements hook_theme_suggestions_HOOK_alter().
 */
function d324theme_theme_suggestions_node_edit_form_alter(array &$suggestions, array $variables) {
  foreach($variables['form'] as $form_item) {
    if(is_array($form_item) && !empty($form_item['widget']['entities']['#attached']['library'])) {
      if (in_array('d324_ace/full_screen_editing', $form_item['widget']['entities']['#attached']['library'])) {
        $suggestions[] = $variables['theme_hook_original'] . '__' . 'full_screen_edit';
        $suggestions[] = $variables['theme_hook_original'] . '__' . $variables['form']['#form_id'] . '__' . 'full_screen_edit';
      }
    }
  }
}

function d324theme_theme_suggestions_form_alter(array &$suggestions, array &$variables) {
  $element = $variables['element'];
  // Add a suggestion based on the form id name.
  if (isset($element['#form_id'])) {
    $suggestions[] = $variables['theme_hook_original'] . '__' . $element['#form_id'];
  }
}
/**
 * Implements hook_theme_suggestions_HOOK_alter().
 */
function d324theme_theme_suggestions_form_element_alter(array &$suggestions, array &$variables) {
  $element = $variables['element'];

  // Add a suggestion based on the element type.
  if (isset($element['#type'])) {
    $suggestions[] = $variables['theme_hook_original'] . '__' . $element['#type'];
  }
}

/**
 * Implements hook_preprocess_input().
 */
function d324theme_preprocess_input(&$variables) {
  $element = $variables['element'];

  // Add the element type to the theme.
  $variables['type'] = $element['#type'];
}

/**
 * Implements hook_preprocess_form_element().
 */
function d324theme_preprocess_form_element(&$variables) {
  $element = $variables['element'];

  // Add required class for checkbox and radio labels.
  if (in_array($element['#type'], ['checkbox', 'radio'])) {
    $variables['label']['#attributes']['class'][] = 'form-check-label';
  }
}

/**
 * Implements hook_preprocess_node_edit_form().
 */
function d324theme_preprocess_d324_ace_frontend_modal_content_wrapper(&$variables) {
  foreach(\Drupal\Core\Render\Element::children($variables['content']['actions']) as $action) {
    switch($action) {
      case "delete":
        $class = 'danger';
        break;
      case "cancel":
        $class = 'warning';
        break;
      default:
        $class = 'primary';
    }
    $variables['content']['actions'][$action]['#attributes']['class'][] = "bg-$class";
  }
}

/**
 * Implements hook_form_FORM_ID_alter().
 */
function d324theme_form_search_block_form_alter(&$form, FormStateInterface $form_state, $form_id) {
  // Update the placeholder and size of field.
  $form['keys']['#title'] = '';
  $form['keys']['#size'] = 20;
  $form['keys']['#placeholder'] = t('Search');
}

/**
 * Implements hook_form_BASE_FORM_ID_alter().
 */
function d324theme_form_node_form_alter(&$form, FormStateInterface $form_state, $form_id) {
  foreach($form as $key => $form_item) {
    if(is_array($form_item) && !empty($form_item['widget']['entities']['#attached']['library'])) {
      if(in_array('d324_ace/full_screen_editing', $form[$key]['widget']['entities']['#attached']['library'])) {
        $form['#uses_full_screen'] = TRUE;
        if(isset($form['group_header'])) {
          $form['group_header'] = [
            '#type' => 'container',
            '#title' => 'Header',
          ];
        }
        if(isset($form['group_categories_tags'])) {
          $form['group_categories_tags'] = [
            '#type' => 'container',
            '#title' => 'Categories / Tags',
          ];
        }
        if(isset($form['meta'])) {
          $form['meta']['#type'] = 'details';
          $form['meta']['#title'] = 'Publishing Details';
        }
        if(isset($form['path']) || isset($form['menu'])) {
          $form['urls'] = [
            '#type' => 'container',
          ];
          if(isset($form['path'])) {
            $form['path']['#group'] = 'urls';
            $form['path']['widget'][0]['#group'] = 'urls';
          }
          if(isset($form['menu'])) {
            $form['menu']['#group'] = 'urls';
            $form['menu']['#open'] = TRUE;
          }
        }
        if(isset($form['field_yoast_seo']) || isset($form['field_meta_tags'])) {
          $form['seo'] = [
            '#type' => 'container',
          ];
          if(isset($form['field_yoast_seo'])) {
            $form['field_yoast_seo']['#group'] = 'seo';
          }
          if(isset($form['field_meta_tags'])) {
            $form['field_meta_tags']['widget'][0]['#group'] = 'seo';
            $form['field_meta_tags']['widget'][0]['#open'] = TRUE;
            $form['field_meta_tags']['widget'][0]['#weight'] = 99;
          }
        }
      }
    }
  }
}
