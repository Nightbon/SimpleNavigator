#ifndef VIEW_H
#define VIEW_H

#include <string>

#include "controller.h"
#include "view_types.h"

namespace s21 {

class View {
 public:
  explicit View(Controller& controller);
  ~View() = default;

  void Run() const;

 private:
  void HandleLoad() const;
  void HandleExport() const;
  void HandleBfsDfs(const std::string&) const;
  void HandleShortestPath() const;
  void HandleAllShortestPaths() const;
  void HandleMst() const;
  void HandleTsp() const;
  void HandleBenchmark() const;
  bool EnsureGraphLoaded() const;

  Controller& controller_;
};

}  // namespace s21

#endif  // VIEW_H