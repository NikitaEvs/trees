/* Ð”Ð°Ð½Ð¾ Ñ‡Ð¸ÑÐ»Ð¾ N < 106 Ð¸ Ð¿Ð¾ÑÐ»ÐµÐ´Ð¾Ð²Ð°Ñ‚ÐµÐ»ÑŒÐ½Ð¾ÑÑ‚ÑŒ Ñ†ÐµÐ»Ñ‹Ñ… Ñ‡Ð¸ÑÐµÐ» Ð¸Ð· [-231..231] Ð´Ð»Ð¸Ð½Ð¾Ð¹ N.
Ð¢Ñ€ÐµÐ±ÑƒÐµÑ‚ÑÑ Ð¿Ð¾ÑÑ‚Ñ€Ð¾Ð¸Ñ‚ÑŒ Ð±Ð¸Ð½Ð°Ñ€Ð½Ð¾Ðµ Ð´ÐµÑ€ÐµÐ²Ð¾ Ð¿Ð¾Ð¸ÑÐºÐ°, Ð·Ð°Ð´Ð°Ð½Ð½Ð¾Ðµ Ð½Ð°Ð¸Ð²Ð½Ñ‹Ð¼ Ð¿Ð¾Ñ€ÑÐ´ÐºÐ¾Ð¼ Ð²ÑÑ‚Ð°Ð²ÐºÐ¸. Ð¢.Ðµ., Ð¿Ñ€Ð¸ Ð´Ð¾Ð±Ð°Ð²Ð»ÐµÐ½Ð¸Ð¸ Ð¾Ñ‡ÐµÑ€ÐµÐ´Ð½Ð¾Ð³Ð¾ Ñ‡Ð¸ÑÐ»Ð° K Ð² Ð´ÐµÑ€ÐµÐ²Ð¾ Ñ ÐºÐ¾Ñ€Ð½ÐµÐ¼ root, ÐµÑÐ»Ð¸ rootâ†’Key â‰¤ K, Ñ‚Ð¾ ÑƒÐ·ÐµÐ» K Ð´Ð¾Ð±Ð°Ð²Ð»ÑÐµÑ‚ÑÑ Ð² Ð¿Ñ€Ð°Ð²Ð¾Ðµ Ð¿Ð¾Ð´Ð´ÐµÑ€ÐµÐ²Ð¾ root; Ð¸Ð½Ð°Ñ‡Ðµ Ð² Ð»ÐµÐ²Ð¾Ðµ Ð¿Ð¾Ð´Ð´ÐµÑ€ÐµÐ²Ð¾ root.
Ð’Ñ‹Ð²ÐµÐ´Ð¸Ñ‚Ðµ ÑÐ»ÐµÐ¼ÐµÐ½Ñ‚Ñ‹ Ð² Ð¿Ð¾Ñ€ÑÐ´ÐºÐµ level-order (Ð¿Ð¾ ÑÐ»Ð¾ÑÐ¼, â€œÐ² ÑˆÐ¸Ñ€Ð¸Ð½Ñƒâ€).*/

#include "iostream"
#include "vector"
#include "queue"
#include "stack"

using namespace std;

template <class T>
struct Nod
{
  Nod *right, *left;
  T key;

  Nod (T element)
  {
    right = NULL;
    left = NULL;
    key = element;
  }
};

template <class T>
class tree
{
 public:
  tree (bool (*ismore)(const T&, const T&))
  {
    comparator = ismore;
  }

  void add (int element)
  {
    if (root == NULL) root = new Nod<T> (element), all_Nods.push(root);
    else
    {
      helproot = root;
      while (true)
      {
        if (comparator(element, helproot->key)/*element >= helproot->key*/)
          if (helproot->right != NULL) helproot = helproot->right;
          else
          {
            helproot->right = new Nod<T> (element);
            all_Nods.push(helproot->right);
            break;
          }
        else
        if (helproot->left != NULL) helproot = helproot->left;
        else
        {
          helproot->left = new Nod<T> (element);
          all_Nods.push(helproot->left);
          break;
        }
      }
    }
  }

  vector<int> print_wise ()
  {
    vector<int> answer;
    queue<Nod<T>*> helpqueue;

    if (root != NULL)
    {
      helpqueue.push(root);

      while (helpqueue.size() != 0)
      {
        if (helpqueue.front()->left != NULL) helpqueue.push(helpqueue.front()->left);
        if (helpqueue.front()->right != NULL) helpqueue.push(helpqueue.front()->right);

        answer.push_back(helpqueue.front()->key);
        helpqueue.pop();
      }
    }


    return answer;
  }

  ~tree()
  {
    while (all_Nods.size() != 0)
    {
      delete all_Nods.top();
      all_Nods.pop();
    }
  }

 private:
  Nod<T>* root = NULL;
  Nod<T>* helproot = NULL;
  bool (*comparator)(const T&, const T&);
  stack<Nod<T>*> all_Nods;
};

template <class T>
bool ismore (const T &a, const T &b)
{
  return a >= b;
}


int main()
{
  int n;
  cin >> n;

  int *mas = new int [n];

  for (int i = 0; i < n; i ++) cin >> mas[i];

  tree<int> easytree (ismore);

  for (int i = 0; i < n; i ++) easytree.add (mas[i]);

  vector<int> answer = easytree.print_wise();
  for (int i = 0; i < answer.size(); i ++) cout << answer[i] << " ";

  delete[] mas;
}