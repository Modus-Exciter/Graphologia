#pragma once
#include <cmath>
#include <vector>
#include "graph.h"

using namespace std;

class NestedMatrix : public Graph
{
	private:
	vector<float> m_arcs;
	size_t m_peak_count;
	
	// Пересчёт двух номеров вершин в номер элемента вектора
	// Ориентированный граф храним в квадратной матрице,
	// неориентированный в треугольной - формулы разные
	size_t index(size_t from, size_t to, size_t peaks) 
	{ 
		// Для использоваиня треугольной матрицы необходимо,
		// чтобы первый номер вершины был меньше второго
		if (!is_oriented() && from > to)
		{
			size_t tmp = from;
			from = to;
			to = tmp;
		}
		return is_oriented() ? from * peaks + to 
			: (2 * peaks - 1 - from) * from / 2 + to - from - 1; 
	}
	
	// То же самое, но количество вершин берём не из параметра, а из поля
	size_t index(size_t from, size_t to) { return index(from, to, m_peak_count); }
	
	// Расчёт размера вектора, необходимого для хранения матрицы: квадратной или треугольной
	size_t calc_size(size_t peaks) { return is_oriented() ? peaks * peaks : peaks * (peaks - 1) / 2; }
	
	// реализацию итератора объявляем внутри матрицы, чтобы использовать приватные члены матрицы
	class Bridge : public ArcBridge
	{
		private:
		NestedMatrix* m_matrix; // Матрица, в которой обходим вершины
		bool m_in;				// Какие дуги обходим - входящие или исходящие
		size_t m_peak;			// Номер вершины, для котрой обходим дуги
		size_t m_position;		// Номер текущей смежной вершины
		
		// В зависимости от того, какие дуги обходим, просто меняем местами пару вершин,
		// всё остальное сделает метод матрицы
		float current_weight()
		{
			return m_in ? m_matrix->weight(m_position, m_peak) : m_matrix->weight(m_peak, m_position);
		}
		
		public:
		Bridge(NestedMatrix* matrix, size_t peak, bool incoming, bool is_end);
		
		void next() override;
		
		bool equals(ArcBridge* other) override;
		
		Arc current() override;
	};
	
	public:
	NestedMatrix(size_t peaks, bool oriented);
	void add_peak() override;
	void rem_peak(size_t peak) override;
	size_t peak_count() override { return m_peak_count; }
	void add_arc(size_t from, size_t to, float weight) override;
	bool rem_arc(size_t from, size_t to) override;
	bool has_arc(size_t from, size_t to) override;
	float weight(size_t from, size_t to) override;
	ArcList outgoing(size_t peak) override;
	ArcList incoming(size_t peak) override;
};