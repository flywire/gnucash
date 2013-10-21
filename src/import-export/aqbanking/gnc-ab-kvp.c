/*
 * gnc-ab-kvp.c --
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, contact:
 *
 * Free Software Foundation           Voice:  +1-617-542-5942
 * 51 Franklin Street, Fifth Floor    Fax:    +1-617-542-2652
 * Boston, MA  02110-1301,  USA       gnu@gnu.org
 */

/**
 * @internal
 * @file gnc-ab-kvp.c
 * @brief AqBanking KVP handling
 * @author Copyright (C) 2002 Christian Stimming <stimming@tuhh.de>
 * @author Copyright (C) 2008 Andreas Koehler <andi5.py@gmx.net>
 */

#include "config.h"
#include "gnc-ui-util.h"

#include "gnc-ab-kvp.h"

#define AB_KEY "hbci"
#define AB_TEMPLATES "template-list"

/* This static indicates the debugging module that this .o belongs to.  */
G_GNUC_UNUSED static QofLogModule log_module = G_LOG_DOMAIN;

static kvp_frame *gnc_ab_get_book_kvp(QofBook *b, gboolean create);

const gchar *
gnc_ab_get_account_accountid(const Account *a)
{
    gchar *id;
    qof_instance_get (QOF_INSTANCE (a),
		      "ab-account-id", &id,
		      NULL);
    return id;
}

void
gnc_ab_set_account_accountid(Account *a, const gchar *id)
{
    xaccAccountBeginEdit(a);
    qof_instance_set (QOF_INSTANCE (a),
		      "ab-account-id", id,
		      NULL);
    xaccAccountCommitEdit(a);
}

const gchar *
gnc_ab_get_account_bankcode(const Account *a)
{
    gchar *code;
    qof_instance_get (QOF_INSTANCE (a),
		      "ab-bank-code", &code,
		      NULL);
    return code;
}

void
gnc_ab_set_account_bankcode(Account *a, const gchar *code)
{
    xaccAccountBeginEdit(a);
    qof_instance_set (QOF_INSTANCE (a),
		      "ab-bank-code", code,
		      NULL);
    xaccAccountCommitEdit(a);
}

guint32
gnc_ab_get_account_uid(const Account *a)
{
    guint64 uid;
    qof_instance_get (QOF_INSTANCE (a),
		      "ab-account-uid", &uid,
		      NULL);
    return (guint32)uid;
}

void
gnc_ab_set_account_uid(Account *a, guint32 uid)
{
    xaccAccountBeginEdit(a);
    qof_instance_set (QOF_INSTANCE (a),
		      "ab-account-uid", (guint64)uid,
		      NULL);
    xaccAccountCommitEdit(a);
}

Timespec
gnc_ab_get_account_trans_retrieval(const Account *a)
{
    Timespec t;
    qof_instance_get (QOF_INSTANCE (a),
		      "ab-trans-retrieval", &t,
		      NULL);
    return t;
}

void
gnc_ab_set_account_trans_retrieval(Account *a, Timespec time)
{
    xaccAccountBeginEdit(a);
    qof_instance_set (QOF_INSTANCE (a),
		      "ab-trans-retrieval", &time,
		      NULL);
    xaccAccountCommitEdit(a);
}

GList *
gnc_ab_get_book_template_list(QofBook *b)
{
    kvp_frame *frame = gnc_ab_get_book_kvp(b, FALSE);
    kvp_value *value = kvp_frame_get_slot(frame, AB_TEMPLATES);
    return kvp_value_get_glist(value);
}

void
gnc_ab_set_book_template_list(QofBook *b, GList *template_list)
{
    kvp_frame *frame = gnc_ab_get_book_kvp(b, TRUE);
    kvp_value *value = kvp_value_new_glist_nc(template_list);
    kvp_frame_set_slot_nc(frame, AB_TEMPLATES, value);
    qof_book_kvp_changed (b);
}

static kvp_frame *
gnc_ab_get_book_kvp(QofBook *b, gboolean create)
{
    kvp_frame *toplevel = qof_book_get_slots(b);
    kvp_frame *result = kvp_frame_get_frame(toplevel, AB_KEY);
    if (!result && create)
    {
        result = kvp_frame_new();
        kvp_frame_add_frame_nc(toplevel, AB_KEY, result);
    }
    return result;
}
