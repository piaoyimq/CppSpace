/*
 * json-example.cpp
 *
 *  Created on: 2016年12月24日
 *      Author: piaoyimq
 */


#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

int  main()
{
  boost::property_tree::ptree ptree_root;
  ptree_root.put("root","me_root");

  boost::property_tree::ptree ptree_items;
  {
      boost::property_tree::ptree pt;  //一棵树
      pt.put<int>("item_1",20);
      pt.put<int>("item_2",40);
      ptree_items.push_back(make_pair("",pt));
  }

  {
      boost::property_tree::ptree pt;//又一棵树
      pt.put("item_1","30");
      pt.put("item_2","60");
      ptree_items.push_back(make_pair("",pt));
  }
  ptree_root.put_child("items", ptree_items );

  boost::property_tree::write_json("./file_path.json",ptree_root);
}

