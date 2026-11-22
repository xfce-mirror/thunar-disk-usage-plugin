/*
 * Copyright (C) 2026 Grigory Tuboltsev
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, see <https://www.gnu.org/licenses/>.
 */


#include "du-view.h"
#include "tdu-window.h"

#include <glib/gi18n-lib.h>



static void
tdu_window_finalize (GObject *object);
static void
tdu_window_destroy (GtkWidget *widget);



struct _TduWindowClass
{
  GtkWindowClass __parent__;
};

struct _TduWindow
{
  GtkWindow __parent__;

  GFile     *location;
  GtkWidget *view;
};



THUNARX_DEFINE_TYPE (TduWindow, tdu_window, GTK_TYPE_WINDOW);



/* one window per folder: a second activation raises the existing one */
static GList *windows = NULL;



static void
tdu_window_class_init (TduWindowClass *klass)
{
  GObjectClass   *gobject_class = G_OBJECT_CLASS (klass);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gobject_class->finalize = tdu_window_finalize;

  widget_class->destroy = tdu_window_destroy;
}



static void
tdu_window_init (TduWindow *window)
{
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 480);
  gtk_window_set_icon_name (GTK_WINDOW (window), "thunar-disk-usage-plugin");

  window->view = du_view_new ();
  gtk_container_set_border_width (GTK_CONTAINER (window), 12);
  gtk_container_add (GTK_CONTAINER (window), window->view);
  gtk_widget_show (window->view);
}



static void
tdu_window_destroy (GtkWidget *widget)
{
  /* a closed window may live on while something holds a reference, but
   * must not be raised again */
  windows = g_list_remove (windows, widget);

  GTK_WIDGET_CLASS (tdu_window_parent_class)->destroy (widget);
}



static void
tdu_window_finalize (GObject *object)
{
  TduWindow *window = TDU_WINDOW (object);

  g_clear_object (&window->location);

  G_OBJECT_CLASS (tdu_window_parent_class)->finalize (object);
}



/**
 * tdu_window_show:
 * @location : the folder to show.
 * @parent   : the Thunar window the action came from, or %NULL.
 *
 * Shows the disk usage window for @location, or raises it if it is open.
 **/
void
tdu_window_show (GFile     *location,
                 GtkWidget *parent)
{
  TduWindow *window;
  GList     *lp;
  gchar     *folder_name;
  gchar     *path;
  gchar     *text;

  g_return_if_fail (G_IS_FILE (location));

  for (lp = windows; lp != NULL; lp = lp->next)
    if (g_file_equal (TDU_WINDOW (lp->data)->location, location))
      {
        /* with the time of the click or key press, so that focus
         * stealing prevention lets the window come up */
        gtk_window_present_with_time (lp->data, gtk_get_current_event_time ());
        return;
      }

  window = g_object_new (TDU_TYPE_WINDOW, NULL);
  window->location = g_object_ref (location);
  windows = g_list_prepend (windows, window);

  if (parent != NULL)
    gtk_window_set_screen (GTK_WINDOW (window), gtk_widget_get_screen (parent));

  path = g_file_get_path (location);

  /* folder names need not be valid UTF-8 */
  if (path != NULL)
    folder_name = g_filename_display_basename (path);
  else
    folder_name = g_file_get_parse_name (location);
  /* TRANSLATORS: window title, %s is the folder name */
  text = g_strdup_printf (_("Disk Usage — %s"), folder_name);
  gtk_window_set_title (GTK_WINDOW (window), text);
  g_free (text);
  g_free (folder_name);

  if (path != NULL)
    du_view_set_path (DU_VIEW (window->view), path);
  g_free (path);

  gtk_window_present (GTK_WINDOW (window));
}
