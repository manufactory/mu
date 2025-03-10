/*
** Copyright (C) 2008-2022-Jan C. Binnema <djcb@djcbsoftware.nl>
**
** This program is free software; you can redistribute it and/or modify it
** under the terms of the GNU General Public License as published by the
** Free Software Foundation; either version 3, or (at your option) any
** later version.
**
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
**
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software Foundation,
** Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
**
*/

#ifndef MU_CMD_HH__
#define MU_CMD_HH__

#include <glib.h>
#include <mu-config.hh>
#include <mu-store.hh>
#include <utils/mu-result.hh>

namespace Mu {
/**
 * execute the 'find' command
 *
 * @param store store object to use
 * @param opts configuration options
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_find(const Mu::Store& store, const MuConfig* opts);

/**
 * execute the 'extract' command
 *
 * @param opts configuration options
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_extract(const MuConfig* opts);

/**
 * execute the 'fields' command
 *
 * @param opts configuration options
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_fields(const MuConfig* opts);

/**
 * execute the 'script' command
 *
 * @param opts configuration options
 * @param err receives error information, or NULL
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_script(const MuConfig* opts);

/**
 * execute the cfind command
 *
 * @param store store object to use
 * @param opts configuration options
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_cfind(const Mu::Store& store, const MuConfig* opts);

/**
 * execute the 'index' command
 *
 * @param store store object to use
 * @param opts configuration options
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_index(Mu::Store& store, const MuConfig* opt);

/**
 * execute the server command
 * @param opts configuration options
 * @param err receives error information, or NULL
 *
 * @return MU_OK (0) if the command succeeds, some error code otherwise
 */
Result<void> mu_cmd_server(const MuConfig* opts);

/**
 * execute some mu command, based on 'opts'
 *
 * @param opts configuration option
 * @param err receives error information, or NULL
 *
 * @return Ok() or some error
 */
Result<void> mu_cmd_execute(const MuConfig* opts);

} // namespace Mu

#endif /*__MU_CMD_H__*/
