#include "node.h"

uint find_path::Node::count = 0;

find_path::Node::Node(bool flag)
    :
      value(count),
      rect(0, 0, 20, 20),
      enabled(flag),
      edges({})
{
    count++;
}

find_path::Node::Node(const QRect& rect, bool flag)
    :
      value(count),
      rect(rect),
      enabled(flag),
      edges({})
{
    count++;
}

find_path::Node::Node(int x, int y, int width, int height, bool flag)
    :
      value(count),
      rect(x, y, width, height),
      enabled(flag),
      edges({})
{
    count++;
}

find_path::Node::Node(const Node& node)
    :
      value(node.value),
      rect(node.rect),
      enabled(node.enabled),
      edges(node.edges)
{}

find_path::Node::Node(Node&& node)
    :
      value(node.value),
      rect(node.rect),
      enabled(node.enabled)
{
    edges = std::move(node.edges);

    node.value = 0;
    node.rect.setRect(0,0,20,20);
    node.enabled = false;
}

find_path::Node::~Node() {
    count--;

    for(Node* node : edges) {
        erase_if(node->edges, [this](Node* edge) { return edge == this; });
    }

    edges.clear();
}

find_path::Node& find_path::Node::operator=(const Node& node) {
    if(&node != this) {
        value = node.value;
        rect = node.rect;
        enabled = node.enabled;
        edges = node.edges;
    }
    return *this;
}

find_path::Node& find_path::Node::operator=(Node&& node) {
    if(&node != this) {
        value = node.value;
        rect = node.rect;
        enabled = node.enabled;
        edges = std::move(node.edges);

        node.value = 0;
        node.rect.setRect(0,0,20,20);
        node.enabled = false;
    }
    return *this;
}

const bool find_path::Node::IsEnabled() const {
    return enabled;
}

void find_path::Node::SetEnabled(const bool flag) {
    enabled = flag;
}

const bool find_path::Node::HasEdge(const Node& to) const {
    return std::find(edges.begin(), edges.end(), &to) != edges.end();
}

const QList<find_path::Node*>& find_path::Node::RelatedEdges() const {
    return edges;
}

const QRect& find_path::Node::GetRect() const {
    return rect;
}

void find_path::Node::SetRect(const QRect& rc) {
    rect = rc;
}

void find_path::Node::AddEdge(Node* to) {
    if(to == nullptr)
        throw Exception("Node object is null");

    edges.push_back(to);
    to->edges.push_back(this);
}

void find_path::Node::RemoveEdge(Node* to) {
    if(to == nullptr)
        throw Exception("Node object is null");

    edges.removeAll(to);
    to->edges.removeAll(this);
}
