/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright (C) 2011 Colin Walters <walters@verbum.org>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * Author: Colin Walters <walters@verbum.org>
 */

#include "config.h"

#include "ht-builtins.h"
#include "hacktree.h"

#include <glib/gi18n.h>

static char *repo_path;
static gboolean ignore_exists;
static gboolean force;

static GOptionEntry options[] = {
  { "repo", 0, 0, G_OPTION_ARG_FILENAME, &repo_path, "Repository path", "repo" },
  { "ignore-exists", 'n', 0, G_OPTION_ARG_NONE, &ignore_exists, "Don't error if file exists", NULL },
  { "force", 'f', 0, G_OPTION_ARG_NONE, &force, "If object exists, relink file", NULL },
  { NULL }
};

gboolean
hacktree_builtin_link_file (int argc, char **argv, const char *prefix, GError **error)
{
  GOptionContext *context;
  gboolean ret = FALSE;
  HacktreeRepo *repo = NULL;
  int i;

  context = g_option_context_new ("- Create a new hard link in the repository");
  g_option_context_add_main_entries (context, options, NULL);

  if (!g_option_context_parse (context, &argc, &argv, error))
    goto out;

  if (repo_path == NULL)
    repo_path = ".";

  repo = hacktree_repo_new (repo_path);
  if (!hacktree_repo_check (repo, error))
    goto out;

  for (i = 0; i < argc-1; i++)
    {
      if (!hacktree_repo_link_file (repo, argv[i+1], ignore_exists, force, error))
        goto out;
    }
 
  ret = TRUE;
 out:
  if (context)
    g_option_context_free (context);
  g_clear_object (&repo);
  return ret;
}
