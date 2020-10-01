#include <cmath>
#include <mem.h>
#include "nestedmatrix.h"

NestedMatrix::NestedMatrix(size_t peaks, bool oriented) : Graph(oriented)
{
	m_peak_count = peaks;
	m_arcs.resize(calc_size(peaks), NAN);
}

void NestedMatrix::add_peak()
{
	// Временный вектор - новая матрица
	vector<float> tmp(calc_size(m_peak_count + 1), NAN);
	
	// Копируем данные из старого вектора в новый
	for (size_t i = 0; i < m_peak_count; i++)
	{
		for (size_t j = is_oriented() ? 0 : i + 1; j < m_peak_count; j++)
		{
			if (i == j)  // пропускаем диагональ
				continue;
			
			tmp[index(i, j, m_peak_count + 1)] = m_arcs[index(i, j)];
		}
	}
	
	// Перемещаем новый вектор в старый
	m_arcs = move(tmp);
	m_peak_count++;
}

void NestedMatrix::rem_peak(size_t peak) 
{
	check_peak(peak);
	
	// Временный вектор - новая матрица
	vector<float> tmp(calc_size(m_peak_count - 1), NAN);
	
	// Копируем данные из старого вектора в новый
	for (size_t i = 0; i < m_peak_count; i++)
	{
		if (i == peak) // пропускаем строку с удаляемой вершиной
			continue;
		
		for (size_t j = is_oriented() ? 0 : i + 1; j < m_peak_count; j++)
		{
			if (i == j || j == peak) // пропускаем столбец с удаляемой вершиной
				continue;
			
			// При удалении вершины индексы могут сместиться - учитываем это
			size_t i2 = i < peak ? i : i - 1;
			size_t j2 = j < peak ? j : j - 1;
			
			tmp[index(i2, j2, m_peak_count - 1)] = m_arcs[index(i, j)];
		}
	}
	
	// Перемещаем новый вектор в старый
	m_arcs = move(tmp);
	m_peak_count--;
}

void NestedMatrix::add_arc(size_t from, size_t to, float weight) 
{
	check_peak(from);
	check_peak(to);
	
	// Защита от добавления дуги, связывающей вершиу с самой собой
	if (from == to)
		throw range_error("peaks must differ");
	
	m_arcs[index(from, to)] = weight;
}

bool NestedMatrix::rem_arc(size_t from, size_t to) 
{
	check_peak(from);
	check_peak(to);
	
	bool ret = !isnan(m_arcs[index(from, to)]);
	
	m_arcs[index(from, to)] = NAN;
	
	return ret;
}

bool NestedMatrix::has_arc(size_t from, size_t to) 
{
	check_peak(from);
	check_peak(to);
	
	return !isnan(m_arcs[index(from, to)]);
}

float NestedMatrix::weight(size_t from, size_t to) 
{
	check_peak(from);
	check_peak(to);
	
	return m_arcs[index(from, to)];
}

// При создании итератора просто передаём, какие дуги обходим - входящие или исходящие
ArcList NestedMatrix::outgoing(size_t peak) 
{
	return ArcList(new Bridge(this, peak, false, false), new Bridge(this, peak, false, true));
}

ArcList NestedMatrix::incoming(size_t peak) 
{
	return ArcList(new Bridge(this, peak, true, false), new Bridge(this, peak, true, true));
}

NestedMatrix::Bridge::Bridge(NestedMatrix* matrix, size_t peak, bool incoming, bool is_end)
{
	m_matrix = matrix;
	m_in = incoming;
	m_peak = peak;
	
	// В зависимости от того, куда указывает итератор,
	// сразу устанавливаем позицию либо на начало, либо на конец столбца или строки
	if (is_end)
		m_position = matrix->peak_count();
	else
		m_position = 0;
	
	// Ищем первую вершину, с которой текущая вершина связана дугой
	while (m_position < matrix->peak_count() && isnan(current_weight()))
		m_position++;
}

void NestedMatrix::Bridge::next()
{
	// Ищем следующуюу вершину, с которой текущая связана дугой
	do
	{
		m_position++;
	} while (m_position < m_matrix->peak_count() && isnan(current_weight()));
}

bool NestedMatrix::Bridge::equals(ArcBridge* other)
{
	Bridge* o = dynamic_cast<Bridge*>(other);
	
	if (o == nullptr)
		return false;
	
	// Просто сравниваем все четыре поля
	return m_matrix == o->m_matrix 
			&& m_in == o->m_in 
			&& m_peak == o->m_peak 
			&& m_position == o->m_position;
}

Arc NestedMatrix::Bridge::current()
{
	Arc ret = { m_position, current_weight() };
	return ret;
}
