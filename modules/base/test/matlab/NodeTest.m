classdef NodeTest < matlab.unittest.TestCase
    methods (Test)
    
        function constructorWithParent(testCase)
            root = ma.Node('root');
            child = ma.Node('child', root);
            testCase.verifyEqual(root.hasChildren(), true);
            testCase.verifyEqual(child.hasParents(), true);
            testCase.verifyEqual(root.name(), 'root');
            testCase.verifyEqual(child.name(), 'child');
            delete(root);
            testCase.verifyEqual(child.hasParents(), false);
        end
        
        function child0(testCase)
            root = ma.Node('root');
            testCase.verifyError(@()root.child(0), 'SWIG:IndexError');
        end

        function child1(testCase)
            root = ma.Node('root');
            ma.Node('child', root);
            child = root.child(1);
            testCase.verifyEqual(child.name(), 'child');
            testCase.verifyEqual(child.hasParents(), true);
            delete(root);
            testCase.verifyEqual(child.name(), 'child');
            testCase.verifyEqual(child.hasParents(), false);
        end

        function children(testCase)
            root = ma.Node('root');
            child1 = ma.Node('child1', root);
            child2 = ma.Node('child2', root);
            children = root.children();
            testCase.verifyEqual(length(children), 2);
            testCase.verifyEqual(children{1}.name(), child1.name());
            testCase.verifyEqual(children{2}.name(), child2.name());
            delete(root);
            testCase.verifyEqual(children{1}.hasParents(), false);
            testCase.verifyEqual(children{2}.hasParents(), false);
            delete(child1);
            delete(child2);
            % Still exist?
            testCase.verifyEqual(children{1}.name(), 'child1');
            testCase.verifyEqual(children{2}.name(), 'child2');
            testCase.verifyEqual(double(children{1}.refcount()), 1);
            testCase.verifyEqual(double(children{2}.refcount()), 1);
        end

        function clearNode(testCase)
            root = ma.Node('root');
            child1 = ma.Node('child1', root);
            child2 = ma.Node('child2', root);
            testCase.verifyEqual(double(root.refcount()), 1);
            testCase.verifyEqual(double(child1.refcount()), 2);
            testCase.verifyEqual(double(child2.refcount()), 2);
            root.clear();
            testCase.verifyEqual(double(root.refcount()), 1);
            testCase.verifyEqual(double(child1.refcount()), 1);
            testCase.verifyEqual(double(child2.refcount()), 1);
        end

        function copyNode(testCase)
            root = ma.Node('root');
            child1 = ma.Node('child1', root);
            child2 = ma.Node('child2', child1);
            other = ma.Node('other');
            sub1 = ma.Node('sub1', other);
            other.copy(root);
            testCase.verifyEqual(sub1.hasParents, false);
            testCase.verifyEqual(double(root.refcount()), 1);
            testCase.verifyEqual(double(child1.refcount()), 2);
            testCase.verifyEqual(double(child2.refcount()), 2);
            testCase.verifyEqual(double(other.refcount()), 1);
            testCase.verifyEqual(double(sub1.refcount()), 1);
            other.child(1).setName('Goal');
            other.child(1).child(1).setName('Goal2');
            testCase.verifyEqual(child1.name(),'child1');
            testCase.verifyEqual(child2.name(),'child2');
        end

        function cloneNode(testCase)
            root = ma.Node('root');
            ma.Node('child1', root);
            other = root.clone();
            other.setName('Foo');
            other.child(1).setName('Bar');
            testCase.verifyEqual(root.hasChildren, true);
            testCase.verifyEqual(other.hasChildren, true);
            testCase.verifyEqual(double(root.refcount()), 1);
            testCase.verifyEqual(double(other.refcount()), 1);
            testCase.verifyEqual(root.name(),'root');
            testCase.verifyEqual(other.name(),'Foo');
            testCase.verifyEqual(root.child(1).name(),'child1');
            testCase.verifyEqual(other.child(1).name(),'Bar');
        end

        function addParent(testCase)
            root = ma.Node('root');
            child1 = ma.Node('child1');
            child1.addParent(root);
            testCase.verifyEqual(double(child1.refcount()), 2);
            testCase.verifyEqual(root.hasChildren, true);
            testCase.verifyEqual(child1.hasParents, true);
            testCase.verifyEqual(root.child(1).name, 'child1');
            delete(root);
            testCase.verifyEqual(double(child1.refcount()), 1);
        end

        function removeParent(testCase)
            root = ma.Node('root');
            child1 = ma.Node('child1', root);
            child1.removeParent(root);
            testCase.verifyEqual(double(child1.refcount()), 1);
            testCase.verifyEqual(root.hasChildren, false);
            testCase.verifyEqual(child1.hasParents, false);
        end

        function setProperty(testCase)
            root = ma.Node('root');
            testCase.verifyEqual(root.property('name').cast('char'), 'root')
            root.setProperty('foo',1);
            testCase.verifyEqual(root.property('foo').cast(), 1);
            root.setProperty('bar',ma.Any(10.0));
            testCase.verifyEqual(root.property('bar').cast(), 10.0);
        end
            
        function copyVariableInCell(testCase)
            nodes = cell(1,3);
            for i = 1:3
                node = ma.Node(['Node',num2str(i)]);
                nodes{i} = node;
            end
            testCase.verifyEqual(nodes{1}.name, 'Node1');
            testCase.verifyEqual(nodes{2}.name, 'Node2');
            testCase.verifyEqual(nodes{3}.name, 'Node3');
        end
        
        function findChildren(testCase)
            root = ma.Node('root');
            node = ma.Node('node',root);
            node.setProperty('foo','bar');
            node.setProperty('hello',true);
            children = root.findChildren(ma.T_Node,'.*',{{'name','node'}});
            testCase.verifyEqual(length(children),1);
            testCase.verifyEqual(children{1}.name,'node');
            children = root.findChildren(ma.T_Node,'.*',{{'foo','bar'}});
            testCase.verifyEqual(length(children),1);
            testCase.verifyEqual(children{1}.name,'node');
            children = root.findChildren(ma.T_Node,'.*',{{'hello',true}});
            testCase.verifyEqual(length(children),1);
            testCase.verifyEqual(children{1}.name,'node');
        end
        
        function findChild(testCase)
            root = ma.Node('root');
            node = ma.Node('node',root);
            node.setProperty('foo','bar');
            node.setProperty('hello',true);
            child = root.findChild(ma.T_Node,'node');
            testCase.verifyEqual(isempty(child), false);
            testCase.verifyEqual(child.name,'node');
            child = root.findChild(ma.T_Node,'anything');
            testCase.verifyEqual(isempty(child), true);
            child = root.findChild(ma.T_Node,'node',{{'foo','bar'}});
            testCase.verifyEqual(isempty(child), false);
            testCase.verifyEqual(child.name,'node');
            child = root.findChild(ma.T_Node,'node',{{'hello',false}});
            testCase.verifyEqual(isempty(child), true);
            child = root.findChild(ma.T_Node,'',{{'foo','bar'}});
            testCase.verifyEqual(isempty(child), false);
            testCase.verifyEqual(child.name,'node');
        end
        
    end
end