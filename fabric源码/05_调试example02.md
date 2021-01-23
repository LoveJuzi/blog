# 调试example02

嗯，通过调试，我发现合约的调用过程相对来说比较简单，而且，合约的调用过程基本上把整个的流程都能串起来。

主要的原因是，这个部分可以反复调用。

## 准备工作

将example02的合约安装好后，我们才能够走我们的调试过程。

## invoke链码的执行内容

嗯，这个链码还是比较简单的，不需要调试，就能看明白。

```golang
func (t *SimpleChaincode) invoke(stub shim.ChaincodeStubInterface, args []string) pb.Response {
	var A, B string    // Entities
	var Aval, Bval int // Asset holdings
	var X int          // Transaction value
	var err error

	if len(args) != 3 {
		return shim.Error("Incorrect number of arguments. Expecting 3")
	}

	A = args[0]
	B = args[1]

	// Get the state from the ledger
	// TODO: will be nice to have a GetAllState call to ledger
	Avalbytes, err := stub.GetState(A)
	if err != nil {
		return shim.Error("Failed to get state")
	}
	if Avalbytes == nil {
		return shim.Error("Entity not found")
	}
	Aval, _ = strconv.Atoi(string(Avalbytes))

	Bvalbytes, err := stub.GetState(B)
	if err != nil {
		return shim.Error("Failed to get state")
	}
	if Bvalbytes == nil {
		return shim.Error("Entity not found")
	}
	Bval, _ = strconv.Atoi(string(Bvalbytes))

	// Perform the execution
	X, err = strconv.Atoi(args[2])
	if err != nil {
		return shim.Error("Invalid transaction amount, expecting a integer value")
	}
	Aval = Aval - X
	Bval = Bval + X
	fmt.Printf("Aval = %d, Bval = %d\n", Aval, Bval)

	// Write the state back to the ledger
	err = stub.PutState(A, []byte(strconv.Itoa(Aval)))
	if err != nil {
		return shim.Error(err.Error())
	}

	err = stub.PutState(B, []byte(strconv.Itoa(Bval)))
	if err != nil {
		return shim.Error(err.Error())
	}

	return shim.Success(nil)
}
```

这个调用函数，借助了参数 stub，这个参数有当前的transaction的相关信息，背书信息等等。另外，这个参数还有函数执行的时候使用的入参。

这里使用 pb.Response，其中 pb 是 protobuf 的缩写，具体的定义如下：

```golang
// A response with a representation similar to an HTTP response that can
// be used within another message.
type Response struct {
	// A status code that should follow the HTTP status codes.
	Status int32 `protobuf:"varint,1,opt,name=status,proto3" json:"status,omitempty"`
	// A message associated with the response code.
	Message string `protobuf:"bytes,2,opt,name=message,proto3" json:"message,omitempty"`
	// A payload that can be used to include metadata with this response.
	Payload              []byte   `protobuf:"bytes,3,opt,name=payload,proto3" json:"payload,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

功能：
    从a账户转转账到b账户
入参：
    1. stub链码的一般性参数（这个参数在稍后的地方详细解释）
    2. args链码的入参
出参：
    如果执行成功，返回成功，如果执行失败，返回错误，这里使用 pb.Response，其中 pb 是 protobuf 的缩写
算法过程：
    1. 获取账户A的可用余额
    2. 获取账户B的可用余额
    3. 获取需要转账的金额X
    4. 从账户A扣除X，给账户B增加X，嗯，这里并没有对金额为负值进行校验
    5. 将账户A的新余额写入到账户A，将账户B的新余额写入到账户B

几个问题：
    1. 如果计算过程中账户A的余额写入成功，账户B的余额写入失败，会发生什么？（更新有问题，暂时不处理了）

## handleTransaction 

```golang
func (handler *Handler) handleTransaction(msg *pb.ChaincodeMessage, errc chan error) {
	// The defer followed by triggering a go routine dance is needed to ensure that the previous state transition
	// is completed before the next one is triggered. The previous state transition is deemed complete only when
	// the beforeInit function is exited. Interesting bug fix!!
	go func() {
		//better not be nil
		var nextStateMsg *pb.ChaincodeMessage

		defer func() {
			handler.triggerNextState(nextStateMsg, errc)
		}()

		errFunc := func(err error, ce *pb.ChaincodeEvent, errStr string, args ...interface{}) *pb.ChaincodeMessage {
			if err != nil {
				payload := []byte(err.Error())
				chaincodeLogger.Errorf(errStr, args...)
				return &pb.ChaincodeMessage{Type: pb.ChaincodeMessage_ERROR, Payload: payload, Txid: msg.Txid, ChaincodeEvent: ce, ChannelId: msg.ChannelId}
			}
			return nil
		}

		// Get the function and args from Payload
		input := &pb.ChaincodeInput{}
		unmarshalErr := proto.Unmarshal(msg.Payload, input)
		if nextStateMsg = errFunc(unmarshalErr, nil, "[%s] Incorrect payload format. Sending %s", shorttxid(msg.Txid), pb.ChaincodeMessage_ERROR.String()); nextStateMsg != nil {
			return
		}

		// Call chaincode's Run
		// Create the ChaincodeStub which the chaincode can use to callback
		stub := new(ChaincodeStub)
		err := stub.init(handler, msg.ChannelId, msg.Txid, input, msg.Proposal)
		if nextStateMsg = errFunc(err, stub.chaincodeEvent, "[%s] Transaction execution failed. Sending %s", shorttxid(msg.Txid), pb.ChaincodeMessage_ERROR.String()); nextStateMsg != nil {
			return
		}
		res := handler.cc.Invoke(stub)

		// Endorser will handle error contained in Response.
		resBytes, err := proto.Marshal(&res)
		if nextStateMsg = errFunc(err, stub.chaincodeEvent, "[%s] Transaction execution failed. Sending %s", shorttxid(msg.Txid), pb.ChaincodeMessage_ERROR.String()); nextStateMsg != nil {
			return
		}

		// Send COMPLETED message to chaincode support and change state
		chaincodeLogger.Debugf("[%s] Transaction completed. Sending %s", shorttxid(msg.Txid), pb.ChaincodeMessage_COMPLETED)
		nextStateMsg = &pb.ChaincodeMessage{Type: pb.ChaincodeMessage_COMPLETED, Payload: resBytes, Txid: msg.Txid, ChaincodeEvent: stub.chaincodeEvent, ChannelId: stub.ChannelId}
	}()
}
```

从参数的类型可以知道，这个函数是接收一个protobuf的消息，然后使用这个消息进行的内容判定。嗯，系统合约和用户合约执行的方式是一致的，但是交流的方式是不同的。

pb.ChaincodeMessage定义如下：

```golang
type ChaincodeMessage struct {
	Type      ChaincodeMessage_Type `protobuf:"varint,1,opt,name=type,proto3,enum=protos.ChaincodeMessage_Type" json:"type,omitempty"`
	Timestamp *timestamp.Timestamp  `protobuf:"bytes,2,opt,name=timestamp,proto3" json:"timestamp,omitempty"`
	Payload   []byte                `protobuf:"bytes,3,opt,name=payload,proto3" json:"payload,omitempty"`
	Txid      string                `protobuf:"bytes,4,opt,name=txid,proto3" json:"txid,omitempty"`
	Proposal  *SignedProposal       `protobuf:"bytes,5,opt,name=proposal,proto3" json:"proposal,omitempty"`
	// event emitted by chaincode. Used only with Init or Invoke.
	// This event is then stored (currently)
	// with Block.NonHashData.TransactionResult
	ChaincodeEvent *ChaincodeEvent `protobuf:"bytes,6,opt,name=chaincode_event,json=chaincodeEvent,proto3" json:"chaincode_event,omitempty"`
	// channel id
	ChannelId            string   `protobuf:"bytes,7,opt,name=channel_id,json=channelId,proto3" json:"channel_id,omitempty"`
	XXX_NoUnkeyedLiteral struct{} `json:"-"`
	XXX_unrecognized     []byte   `json:"-"`
	XXX_sizecache        int32    `json:"-"`
}
```

关键字段说明：
    Type: 链码消息的类型，嗯，这个字段是用来标识应该使用何种类型的方法来处理这个链码消息，在这里，这个类型的值是“ChaincodeMessage_TRANSACTION”
    Payload: 交易的参数内容
    Txid: transaction的id，从这个字段，我们分析，交易的信息构建过程应该是在调用这个方法之前
    Proposal: 提案，这里面有签名信息
    ChannelId: 链id

函数功能：
    完成链码交易的执行
入参：
    1. pb.ChaincodeMessage所有有关交易的信息
    2. errc 如果出错，那么向这个通道写入错误信息
出参：
    如果交易执行成功，返回成功信息，如果交易失败，返回错误信息
算法过程：
    1. 启动一个子协程，嗯，应该是为了快速响应其他交易
    2. 解析交易的参数
    3. 生成stub（唉，就是在这里生成的），需要使用参数：handle, chainId, txId, input, proposal
    4. 调用 handle.cc.Invoke 方法， Invoke 方法就是合约的一个入口方法
    5. 解析返回值，返回对应的信息

几个问题：
    1. handle.cc 是如何注册进去的？

## chaincode的启动过程

嗯，这里我们关心的是，链码的启动过程，至于，链码是如何启动的，嗯，这个过程是需要在后面研究的，简单的说就是，如何启动的

这里，由于没有容器，所以，我们主动启动一个链码处理过程，因为这里只是一个简单的例子。

一个重要的包 shim

### shim

```golang
// Package shim provides APIs for the chaincode to access its state
// variables, transaction context and call other chaincodes.
package shim
```

shim 包就是负责链码如何访问账本，访问交易上下文，或者调用其他的链码（嗯，这个我一直没有试过，不知道是个什么流程）

### Start 

```golang
// chaincodes.
func Start(cc Chaincode) error {
	// If Start() is called, we assume this is a standalone chaincode and set
	// up formatted logging.
	SetupChaincodeLogging()

	chaincodename := viper.GetString("chaincode.id.name")
	if chaincodename == "" {
		return errors.New("error chaincode id not provided")
	}

	err := factory.InitFactories(factory.GetDefaultOpts())
	if err != nil {
		return errors.WithMessage(err, "internal error, BCCSP could not be initialized with default options")
	}

	//mock stream not set up ... get real stream
	if streamGetter == nil {
		streamGetter = userChaincodeStreamGetter
	}

	stream, err := streamGetter(chaincodename)
	if err != nil {
		return err
	}

	err = chatWithPeer(chaincodename, stream, cc)

	return err
}
```

启动过程还是比较简单的。

ChainCode接口定义如下：

```golang
// Chaincode interface must be implemented by all chaincodes. The fabric runs
// the transactions by calling these functions as specified.
type Chaincode interface {
	// Init is called during Instantiate transaction after the chaincode container
	// has been established for the first time, allowing the chaincode to
	// initialize its internal data
	Init(stub ChaincodeStubInterface) pb.Response

	// Invoke is called to update or query the ledger in a proposal transaction.
	// Updated state variables are not committed to the ledger until the
	// transaction is committed.
	Invoke(stub ChaincodeStubInterface) pb.Response
}
```

接口方法说明：
    1. Init：用来在实例化的时候初始化链码的账本
    2. Invoke：用来处理业务流程的地方，这里负责读写的调用

函数功能：
    启动链码的处理服务
入参：
    1. cc 链码对象
出参：
    void
算法过程：
    1. 设置链码的日志相关信息
    2. 获取链码的名字，嗯，这里是怎么操作的？环境变量？配置文件？不管怎样，反正我是能获得一个我想要的值
    3. 嗯，不知道工厂是干嘛用的
    4. 获取一个stream，嗯，具体的实现是方法“userChaincodeStreamGetter”完成的，这个比较重要，一会再说
    5. 将链码名字，stream，以及链码的实现，三个参数，构建一个流循环，以后凡是涉及到该链码的工作都会进入这个流循环，具体实现一会再说

### userChaincodeStreamGetter

```golang
//the non-mock user CC stream establishment func
func userChaincodeStreamGetter(name string) (PeerChaincodeStream, error) {
	flag.StringVar(&peerAddress, "peer.address", "", "peer address")
	if viper.GetBool("peer.tls.enabled") {
		keyPath := viper.GetString("tls.client.key.path")
		certPath := viper.GetString("tls.client.cert.path")

		data, err1 := ioutil.ReadFile(keyPath)
		if err1 != nil {
			err1 = errors.Wrap(err1, fmt.Sprintf("error trying to read file content %s", keyPath))
			chaincodeLogger.Errorf("%+v", err1)
			return nil, err1
		}
		key = string(data)

		data, err1 = ioutil.ReadFile(certPath)
		if err1 != nil {
			err1 = errors.Wrap(err1, fmt.Sprintf("error trying to read file content %s", certPath))
			chaincodeLogger.Errorf("%+v", err1)
			return nil, err1
		}
		cert = string(data)
	}

	flag.Parse()

	chaincodeLogger.Debugf("Peer address: %s", getPeerAddress())

	// Establish connection with validating peer
	clientConn, err := newPeerClientConnection()
	if err != nil {
		err = errors.Wrap(err, "error trying to connect to local peer")
		chaincodeLogger.Errorf("%+v", err)
		return nil, err
	}

	chaincodeLogger.Debugf("os.Args returns: %s", os.Args)

	chaincodeSupportClient := pb.NewChaincodeSupportClient(clientConn)

	// Establish stream with validating peer
	stream, err := chaincodeSupportClient.Register(context.Background())
	if err != nil {
		return nil, errors.WithMessage(err, fmt.Sprintf("error chatting with leader at address=%s", getPeerAddress()))
	}

	return stream, nil
}
```

嗯，从这个函数也可以知道，如果某个peer没有安装链码，那么这个peer是不可能能够执行链码的，这个和EOS是不同的。

函数功能：
    生成一个PeerChaincodeStream，用于chaincode执行实体和对应的peer节点通信
入参：
    链码的名字
出参:
    PeerChaincodeStream
算法过程：
    1. 获取peer节点监听链码的服务端口
    2. 生成一个grpc客户端的连接
    3. 使用客户端连接生成一个链码客户端
    4. 重要的一步，使用这个客户端生成一个流，用于客户端和服务端的通信

### chatWithPeer

```golang
func chatWithPeer(chaincodename string, stream PeerChaincodeStream, cc Chaincode) error {
	// Create the shim handler responsible for all control logic
	handler := newChaincodeHandler(stream, cc)
	defer stream.CloseSend()

	// Send the ChaincodeID during register.
	chaincodeID := &pb.ChaincodeID{Name: chaincodename}
	payload, err := proto.Marshal(chaincodeID)
	if err != nil {
		return errors.Wrap(err, "error marshalling chaincodeID during chaincode registration")
	}

	// Register on the stream
	chaincodeLogger.Debugf("Registering.. sending %s", pb.ChaincodeMessage_REGISTER)
	if err = handler.serialSend(&pb.ChaincodeMessage{Type: pb.ChaincodeMessage_REGISTER, Payload: payload}); err != nil {
		return errors.WithMessage(err, "error sending chaincode REGISTER")
	}

	// holds return values from gRPC Recv below
	type recvMsg struct {
		msg *pb.ChaincodeMessage
		err error
	}
	msgAvail := make(chan *recvMsg, 1)
	errc := make(chan error)

	receiveMessage := func() {
		in, err := stream.Recv()
		msgAvail <- &recvMsg{in, err}
	}

	go receiveMessage()
	for {
		select {
		case rmsg := <-msgAvail:
			switch {
			case rmsg.err == io.EOF:
				err = errors.Wrapf(rmsg.err, "received EOF, ending chaincode stream")
				chaincodeLogger.Debugf("%+v", err)
				return err
			case rmsg.err != nil:
				err := errors.Wrap(rmsg.err, "receive failed")
				chaincodeLogger.Errorf("Received error from server, ending chaincode stream: %+v", err)
				return err
			case rmsg.msg == nil:
				err := errors.New("received nil message, ending chaincode stream")
				chaincodeLogger.Debugf("%+v", err)
				return err
			default:
				chaincodeLogger.Debugf("[%s]Received message %s from peer", shorttxid(rmsg.msg.Txid), rmsg.msg.Type)
				err := handler.handleMessage(rmsg.msg, errc)
				if err != nil {
					err = errors.WithMessage(err, "error handling message")
					return err
				}

				go receiveMessage()
			}

		case sendErr := <-errc:
			if sendErr != nil {
				err := errors.Wrap(sendErr, "error sending")
				return err
			}
		}
	}
}
```

## 一点小总结

1. 调试网络通信的过程，我发现寻找到事件循环是一个重要的点，找到事件循环，就能找到对应的注册的执行函数。
2. 需要找到连接的响应地方，这样可以比较容易的找到事件循环。

## 