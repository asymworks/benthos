/*
 * Copyright (C) 2012 Asymworks, LLC.  All Rights Reserved.
 * www.asymworks.com / info@asymworks.com
 *
 * This file is part of the Benthos Dive Log Package (benthos-log.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "compositelistview.hpp"

CompositeListView::CompositeListView(QWidget * parent)
	: QWidget(parent), m_view(0)
{
}

CompositeListView::~CompositeListView()
{
}

void CompositeListView::clearSelection()
{
	QItemSelectionModel * sm = selectionModel();
	if (sm)
		sm->clear();

	emit currentChanged(QModelIndex(), QModelIndex());
	emit selectionChanged(QItemSelection(), QItemSelection());
}

QModelIndex CompositeListView::currentIndex() const
{
	if (! m_view)
		return QModelIndex();
	return m_view->currentIndex();
}

void CompositeListView::onActivated(const QModelIndex & index)
{
	emit activated(index);
}

void CompositeListView::onClicked(const QModelIndex & index)
{
	emit clicked(index);
}

void CompositeListView::onCurrentChanged(const QModelIndex & current, const QModelIndex & previous)
{
	emit currentChanged(current, previous);
}

void CompositeListView::onDoubleClicked(const QModelIndex & index)
{
	emit doubleClicked(index);
}

void CompositeListView::onSelectionChanged(const QItemSelection & selected, const QItemSelection & deselected)
{
	emit selectionChanged(selected, deselected);
}

QItemSelectionModel * CompositeListView::selectionModel() const
{
	if (m_view)
		return m_view->selectionModel();
	return 0;
}

void CompositeListView::setCurrentIndex(const QModelIndex & index)
{
	if (m_view)
		m_view->setCurrentIndex(index);
}

void CompositeListView::setView(QAbstractItemView * v)
{
	if (m_view)
	{
		disconnect(m_view, SIGNAL(activated(const QModelIndex &)), this, SLOT(onActivated(const QModelIndex &)));
		disconnect(m_view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClicked(const QModelIndex &)));
		disconnect(m_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

		if (m_view->selectionModel())
		{
			disconnect(m_view->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
				this, SLOT(onCurrentChanged(const QModelIndex &, const QModelIndex &)));

			disconnect(m_view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
				this, SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
		}
	}

	m_view = v;

	if (m_view)
	{
		connect(m_view, SIGNAL(activated(const QModelIndex &)), this, SLOT(onActivated(const QModelIndex &)));
		connect(m_view, SIGNAL(clicked(const QModelIndex &)), this, SLOT(onClicked(const QModelIndex &)));
		connect(m_view, SIGNAL(doubleClicked(const QModelIndex &)), this, SLOT(onDoubleClicked(const QModelIndex &)));

		if (m_view->selectionModel())
		{
			connect(m_view->selectionModel(), SIGNAL(currentChanged(const QModelIndex &, const QModelIndex &)),
				this, SLOT(onCurrentChanged(const QModelIndex &, const QModelIndex &)));

			connect(m_view->selectionModel(), SIGNAL(selectionChanged(const QItemSelection &, const QItemSelection &)),
				this, SLOT(onSelectionChanged(const QItemSelection &, const QItemSelection &)));
		}
	}
}

QAbstractItemView * CompositeListView::view() const
{
	return m_view;
}
